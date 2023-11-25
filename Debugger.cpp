#include "Debugger.h"
#include "Utils.h"

Debugger::Debugger(
	_In_ const char* TargetExeName,
	_In_ HandleDebuggerEvent Handler,
	_In_ void* Ctx) {


	do {
		m_Ctx = Ctx;
		m_TargetExeName = std::string(TargetExeName);
		m_Handler = Handler;

	} while (false);
	
}

Debugger::~Debugger() {

}

void Debugger::Run() {
	do {
		bool res = CreateTargetProcess();
		if (!res) {
			ERROR_LOG("Failed to create target process");
			break;
		}

		res = RunDebuggerLoop();
		if (!res) {
			ERROR_LOG("Error in debugger loop");
		}

	} while (false);
}

bool Debugger::ReadTargetMemory(
	_In_ char* Addr,
	_In_ ULONG Size,
	_Out_ char* Buffer) {

	BOOL res = 
		ReadProcessMemory(
			m_TargetHandle, 
			Addr, 
			Buffer, 
			Size, 
			nullptr
		);
	if (!res) {
		ERROR_LOG("Failed to read target memory");
	}
	return res;
}
bool Debugger::WriteTargetMemory(
	_In_ char* Addr,
	_In_ ULONG Size,
	_In_ char* Buffer) {
	
	BOOL res = WriteProcessMemory(
		m_TargetHandle, 
		Addr, 
		Buffer, 
		Size, 
		nullptr
	);
	if (!res) {
		ERROR_LOG("Failed to write target memory");
	}
	return res;
}


bool Debugger::HandleEvent(
	DEBUG_EVENT& Event,
	_Out_ bool& Continue) {

	Continue = true;
	bool res = true;

	switch (Event.dwDebugEventCode) {
		case EXIT_PROCESS_DEBUG_EVENT: {
			res = HandleTerminateProcess(Event.u.ExitProcess);
			if (!res) {
				ERROR_LOG("Failed to terminate process");
			}
			
		}break;
		case EXCEPTION_DEBUG_EVENT: {
			res = HandleException(
				Event.u.Exception,
				Continue
			);
			if (!res) {
				ERROR_LOG("Failed to handle exception");
			}
		}break;
	}

	return res;
}

bool Debugger::RunDebuggerLoop() {
	bool res = true;
	
	while (true) {
		DEBUG_EVENT event;
		res = WaitForDebugEvent(
			&event, 
			INFINITE
		);
		if (!res) {
			ERROR_LOG("Failed to wait for debugger events");
			break;
		}

		bool _continue;
		res = HandleEvent(
			event, 
			_continue);
		if (!res) {
			ERROR_LOG("Failed to handle event");
			break;
		}

		if (!_continue) {
			TerminateProcess(m_TargetHandle, -1);
		}


		res = ContinueDebugEvent(
			event.dwProcessId, 
			event.dwThreadId, 
			DBG_CONTINUE
		);
		if (!res) {
			ERROR_LOG("Failed to continue debugger excecution");
			break;
		}
		
	}
	return res;
}

bool Debugger::CreateTargetProcess() {
	/*
	* TODO: Add option to specify args for the target process
	*/

	STARTUPINFOA info;
	ZeroMemory(&info, sizeof(info));

	info.cb = sizeof(info);

	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));

	BOOL res = CreateProcessA(
		(LPCSTR)m_TargetExeName.c_str(), 
		(LPSTR)m_TargetExeName.c_str(), 
		nullptr, 
		nullptr, 
		FALSE, 
		DEBUG_ONLY_THIS_PROCESS, 
		nullptr, 
		nullptr, 
		&info, 
		&processInfo
	);

	m_TargetHandle = processInfo.hProcess;

	return res;
}

bool Debugger::HandleTerminateProcess(_In_ EXIT_PROCESS_DEBUG_INFO& Info) {
	auto event = EProcessTerminated();
	bool res = PushGuiEvent(event);
	if (!res) {
		ERROR_LOG("Failed to push event to handler");
	}

	return true;
}

bool Debugger::HandleBreakPointException(_In_ EXCEPTION_DEBUG_INFO& Info) {
	
	auto event = EBreakPointEvent();
	bool res = PushGuiEvent(event);
	if (!res) {
		ERROR_LOG("Failed to push event to handler");
	}
	return true;
}

bool Debugger::HandleFaltalException(_In_ EXCEPTION_DEBUG_INFO& Info) {
	/*
	* TODO: Add an exception type
	*/

	auto event = EException();
	bool res = PushGuiEvent(event);
	if (!res) {
		ERROR_LOG("Failed to push event to handler");
	}
	return true;
}

bool Debugger::HandleException(
	_In_ EXCEPTION_DEBUG_INFO& Info,
	_Out_ bool& Continue) {

	Continue = false;
	bool res = true;

	switch (Info.ExceptionRecord.ExceptionCode) {
		case STATUS_BREAKPOINT: {
			res = HandleBreakPointException(Info);
			if (!res) {
				ERROR_LOG("Failed to handle breakpoint");
			}
			Continue = true;

		}break;

		default: {
			res = HandleFaltalException(Info);
			if (!res) {
				ERROR_LOG("Failed to handle fatal exception");
			}
		}break;
	}

	return res;
}

bool Debugger::PushGuiEvent(_In_ DebuggerEvent& Event) {
	return m_Handler(
		Event, 
		m_Ctx
	);
}