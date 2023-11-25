#include "ConsoleGui.h"
#include <iostream>

ConsoleGui::ConsoleGui() {

}
ConsoleGui::~ConsoleGui() {

}

void ConsoleGui::Run() {
	while(true){}
}

bool ConsoleGui::Visit(EBreakPointEvent* Breakpoint) {
	std::cout << "Breakpoint\n";
	return true;
}
bool ConsoleGui::Visit(EException* Exception) {
	std::cout << "Exception\n";
	return true;
}
bool ConsoleGui::Visit(EProcessTerminated* Exception) {
	std::cout << "Terminate\n";
	return true;
}