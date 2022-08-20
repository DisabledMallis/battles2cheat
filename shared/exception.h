#ifndef SHARED_EXCEPTION
#define SHARED_EXCEPTION

#include <Windows.h>

namespace exception {
	LONG WINAPI exceptionHandler(_In_ struct _EXCEPTION_POINTERS* ExceptionInfo);
};

#endif // !SHARED_EXCEPTION
