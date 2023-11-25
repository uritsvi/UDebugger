#pragma once

#include <Windows.h>
#include <string>
#include <sal.h>

#include "DebuggerGui.h"


typedef bool(*HandleDebuggerEvent)(
		_In_ DebuggerEvent& Event, 
		_In_ void* Ctx
	);

class Debugger
{
public:
	Debugger() = default;
	Debugger(
		_In_ const char* TargetExeName,
		_In_ HandleDebuggerEvent Handler,
		_In_ void* Ctx
	);
	
	~Debugger();

	void Run();

private:
	bool CreateTargetProcess();
	bool RunDebuggerLoop();

	bool HandleEvent(
		_In_ DEBUG_EVENT& Event, 
		_Out_ bool& Continue
	);

	bool ReadTargetMemory(
		_In_ char* Addr, 
		_In_ ULONG Size, 
		_Out_ char* Buffer
	);
	bool WriteTargetMemory(
		_In_ char* Addr,
		_In_ ULONG Size,
		_In_ char* Buffer
	);

	bool HandleTerminateProcess(_In_ EXIT_PROCESS_DEBUG_INFO& Info);
	bool HandleException(
		_In_ EXCEPTION_DEBUG_INFO& Info, 
		_Out_ bool& Continue
	);

	bool HandleBreakPointException(_In_ EXCEPTION_DEBUG_INFO& Info);
	bool HandleFaltalException(_In_ EXCEPTION_DEBUG_INFO& Info);

	bool PushGuiEvent(_In_ DebuggerEvent& Event);

private:
	std::string m_TargetExeName;
	UINT m_TargetPid;

	HANDLE m_TargetHandle;

	HandleDebuggerEvent m_Handler;
	void* m_Ctx;
};

