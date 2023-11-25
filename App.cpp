#include "App.h"
#include "Utils.h"
#include "ConsoleGui.h"

bool CallbackHandleDebuggerEvent(
	_In_ DebuggerEvent& Event, 
	_In_ void* Ctx) {
	
	auto app = reinterpret_cast<App*>(Ctx);
	return app->SelfHandleDebuggerEvent(Event);
}

DWORD DebuggerThread(_In_ LPVOID Ctx) {
	auto app = reinterpret_cast<App*>(Ctx);
	app->DebeggerThread();

	return 0;
}

bool App::SelfHandleDebuggerEvent(_In_ DebuggerEvent& Event) {
	return Event.Accept(m_Gui.get());
}

void App::DebeggerThread() {
	m_Debugger.Run();
}

App::App(
	_In_ int Argc,
	_In_ char* Argv[]) {

	if (Argc < 2) {
		Utils::ExitWithError("Not enough parameters");
		return;
	}
	m_Debugger = Debugger(
		Argv[1],
		CallbackHandleDebuggerEvent,
		this
	);

	m_Gui = 
		std::make_shared<ConsoleGui>();

}
App::~App() {
}

void App::Run() {
	do {
		bool res = CreateThread(
			nullptr,
			0,
			DebuggerThread,
			this,
			0,
			0
		);
		if (!res) {
			ERROR_LOG("Failed to create debugger thread");
			break;
		}

		m_Gui->Run();

	} while (false);

}