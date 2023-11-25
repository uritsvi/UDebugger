#include <Windows.h>

#include "Utils.h"

void Utils::ExitWithError(_In_ const char* Msg) {
	ERROR_LOG(Msg);
	exit(-1);
}
