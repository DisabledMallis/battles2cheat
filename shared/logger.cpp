#include "logger.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <mutex>

std::vector<LogMessage> logged_messages;

void ilog(const char* fmt, ...) {
    va_list lst;
    va_start(lst, fmt);
    vilog(LogLevel::Info, fmt, lst);
    va_end(lst);
}
void ilog(LogLevel level, const char* fmt, ...) {
    va_list lst;
    va_start(lst, fmt);
    vilog(level, fmt, lst);
    va_end(lst);
}
std::mutex logMutex;
void vilog(LogLevel level, const char* fmt, va_list lst) {
    std::lock_guard<std::mutex> locker(logMutex);
    int required = vsnprintf(nullptr, 0, fmt, lst) + 1;
    if (required < 2) {
        return;
    }
    char* buffer = new char[required];
    vsprintf_s(buffer, required, fmt, lst);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level) {
    case LogLevel::Success: {
        SetConsoleTextAttribute(hConsole, 10);
        break;
    }
    case LogLevel::Warning: {
        SetConsoleTextAttribute(hConsole, 14);
        break;
    }
    case LogLevel::Error: {
        SetConsoleTextAttribute(hConsole, 12);
        break;
    }
    case LogLevel::Info:
    default: {
        SetConsoleTextAttribute(hConsole, 7);
        break;
    }
    }
    std::cout << buffer << std::endl << std::flush;
    LogMessage log_msg;
    log_msg.level = level;
    log_msg.message = buffer;
    logged_messages.push_back(log_msg);
}