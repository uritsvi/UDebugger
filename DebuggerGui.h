#pragma once

#include <sal.h>

class DebuggerGui;

class DebuggerEvent {
public:
	virtual bool Accept(_In_ DebuggerGui* App) = 0;
};

class EBreakPointEvent : public DebuggerEvent {

public:
	bool Accept(_In_ DebuggerGui* App);
};

class EException : public DebuggerEvent {
public:
	bool Accept(_In_ DebuggerGui* App);
};

class EProcessTerminated : public DebuggerEvent {
public:
	bool Accept(_In_ DebuggerGui* App);
};

class DebuggerGui {
public:
	virtual void Run() = 0;

	virtual bool Visit(EBreakPointEvent* Breakpoint) = 0;
	virtual bool Visit(EException* Exception) = 0;
	virtual bool Visit(EProcessTerminated* Exception) = 0;
};

