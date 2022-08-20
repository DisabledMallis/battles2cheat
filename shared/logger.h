#ifndef LOGGER
#define LOGGER
#include <cstdarg>

#define STR(x) #x

#include <Windows.h>
#define ALERTA(MSG) MessageBoxA(nullptr, MSG, "Injector", MB_OK);
#define ALERTW(MSG) MessageBoxW(nullptr, MSG, L"Injector", MB_OK);

enum class LogLevel { Info, Warning, Error, Success };
struct LogMessage {
	LogLevel level;
	char* message; //Cannot be deallocated
};

void ilog(const char* fmt, ...);
void ilog(LogLevel level, const char* fmt, ...);
void vilog(LogLevel level, const char* fmt, va_list args);
void dumpLogs();

#endif