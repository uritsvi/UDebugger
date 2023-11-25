#pragma once

#include <sal.h>

#include <memory>

#include "Debugger.h"
#include "DebuggerGui.h"


class App
{
public:
	App(_In_ int Argc, 
		_In_ char* Argv[]
	);
	~App();

	void Run();
	void DebeggerThread();

	bool SelfHandleDebuggerEvent(_In_ DebuggerEvent& Event);

private:
	Debugger m_Debugger;
	std::shared_ptr<DebuggerGui> m_Gui;
};

