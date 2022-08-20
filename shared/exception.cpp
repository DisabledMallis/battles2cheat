#include "exception.h"

#include <vector>
#include <string>
#include <DbgHelp.h>
#include "files.h"
#include "logger.h"

extern std::vector<LogMessage> logged_messages;

LONG WINAPI exception::exceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
    ilog("Crash detected, dumping info...");
    HANDLE currentProcess = GetCurrentProcess();
    DWORD currentPID = GetCurrentProcessId();
    HANDLE dumpFile = files::open_file(files::getUniformDir() + "/last_crash.dmp");
    MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
        MiniDumpWithFullMemory |
        MiniDumpWithHandleData |
        MiniDumpWithUnloadedModules |
        MiniDumpWithThreadInfo |
        MiniDumpIgnoreInaccessibleMemory |
        MiniDumpWithIptTrace);
    DWORD threadId = GetCurrentThreadId();
    MINIDUMP_EXCEPTION_INFORMATION dumpExceptionInfo = {
        threadId,
        ExceptionInfo,
        FALSE
    };
    bool dumpMade = MiniDumpWriteDump(currentProcess, currentPID, dumpFile, dumpType, &dumpExceptionInfo, NULL, NULL);
    ilog("Dump step completed, saving logs...");
    ilog("Dump successful: %x", dumpMade);
    HANDLE logFile = files::open_file(files::getUniformDir() + "/crashlog.txt");
    std::string toSave = "";
    toSave += "---------BEGINNING OF LOGS---------\n";
    for (auto logMsg : logged_messages) {
        toSave += std::string(logMsg.message) + "\n";
    }
    toSave += "---------END OF LOGS---------\n";
    files::save_file(logFile, (char*)toSave.c_str(), toSave.length());
    if (dumpMade) {
        ilog("Dump created");
        ALERTA("B2C has crashed, and a dump has been created. Please send this info to the B2C developers and nobody else. This dump file includes critical information that can be used to patch B2C from being used in the game.")
    }
    else {
        ilog("Dump failed");
        ALERTA("B2C has crashed, but a dump could not be created. Please restart B2C!");
    }
    return 0;
}