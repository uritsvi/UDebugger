#pragma once

#include "DebuggerGui.h"
 
class ConsoleGui : public DebuggerGui
{
public:
	ConsoleGui();
	~ConsoleGui();

	void Run();

	bool Visit(EBreakPointEvent* Breakpoint) override;
	bool Visit(EException* Exception) override;
	bool Visit(EProcessTerminated* Exception) override;
};

