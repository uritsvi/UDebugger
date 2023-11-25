#pragma once

#include <iostream>

#ifdef _DEBUG

#define ERROR_LOG(Msg) std::cout << "\033[0;31m" << Msg << "\n" << "\033[0m"

#else

#define ERROR_LOG(Msg)

#endif

//#define UI_LOG(Msg) std::cout << Msg << "\n"
//#define WUI_LOG(Msg) std::wcout << Msg << "\n"

typedef int(*ThreadProc)(_In_ void* Ctx);

class Utils
{
public:
	static void ExitWithError(_In_ const char* Msg);
};

