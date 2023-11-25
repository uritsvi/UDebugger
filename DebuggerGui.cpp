#include "DebuggerGui.h"

bool EBreakPointEvent::Accept(_In_ DebuggerGui* App) {
	return App->Visit(this);
}

bool EException::Accept(_In_ DebuggerGui* App) {
	return App->Visit(this);
}

bool EProcessTerminated::Accept(_In_ DebuggerGui* App) {
	return App->Visit(this);
}
