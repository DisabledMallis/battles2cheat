#include <Windows.h>
#include <thread>
#include <iostream>
#include "hooks/hook_manager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"

#include "render/render_mgr.h"
#if CHEAT_ONLY == 0
#include "decryptor.h"
#include "player_info.h"
#include "js_console.h"
//#include "redshift_display.h"
#include "seshgen/session_generator.h"
#include "inject_playground.h"
#include "win_bot.h"
#include "network/packet_viewer.h"
#include "network/transaction_viewer.h"
#include "network/request_viewer.h"
#endif
#include "cheat_menu.h"
#include "js_wrapper.h"
#include "bfbproxy/proxy.h"
#include <nuv.h>
#include <fstream>
#include <auth.h>
#include <exception.h>
#include <utils/account_manager.h>
#include "seshgen/session_generator.h"

/*
//Failed anti anti dbg attempt
uint64_t dbg_original;
void* Hook_DbgUiRemoteBreakin() {
	//Theres no debugger here :)
	return nullptr;
}
*/

uint64_t oCreateFileW;
HANDLE __fastcall CreateFileWCallback(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
	//std::wcout << "CreateFileW call to: \"" << lpFileName << "\"" << std::endl;

	/*
	std::wstring wFileName = lpFileName;
	if (wFileName.find(L"current.session") != std::wstring::npos) {
		ilog("Sussy file!");
		std::cin.get();
	}
	*/

	return PLH::FnCast(oCreateFileW, CreateFileWCallback)(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

#include <game_instance.h>
#include <CLoc.h>
extern game_instance* g_instance;
extern auth_info* toForce;
uint64_t oRaiseException = 0;
VOID WINAPI hkRaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, _In_reads_opt_(nNumberOfArguments) CONST ULONG_PTR* lpArguments) {
	return;
	//return PLH::FnCast(oRaiseException, RaiseException)(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
}

uint64_t oSetUnhandledExceptionFilter = 0;
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI hkSetUnhandledExceptionFilter(_In_opt_ LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
	return lpTopLevelExceptionFilter;
}

int start_cheat(HINSTANCE dll_instance) {
	ilog("Cheat started");
#ifdef _DEBUG
	AllocConsole();
	SetConsoleTitleA("Hacker console >:)");
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	std::cout.clear();
#endif
	ilog("Battles 2 Cheat by DisabledMallis https://github.com/DisabledMallis");

	//We want to handle exceptions, this allows us to attach a debugger & use breakpoints,
	//as well as if the game does actually encounter a fatal error, the data wont be dumped & sent to NK
	//As of 1.2.2 this causes issues, i think the game sets the exception handler to catch an exception? Not sure
	/*
	SetUnhandledExceptionFilter(exception::exceptionHandler);
	PLH::x64Detour* plhSUEF = new PLH::x64Detour((uint64_t)SetUnhandledExceptionFilter, (uint64_t)hkSetUnhandledExceptionFilter, &oSetUnhandledExceptionFilter);
	plhSUEF->hook();
	ilog("Exception handler set");

	PLH::x64Detour* plhRaiseException = new PLH::x64Detour((uint64_t)RaiseException, (uint64_t)hkRaiseException, &oRaiseException);
	plhRaiseException->hook();
	ilog("No more raise exception!");
	*/

	//account_manager::account_data accData = account_manager::logged_in();
	//if (accData == account_manager::NULL_ACCOUNT) {
		//if (MessageBoxA(nullptr, "No BTDB2 account is logged in, create one automatically?", "B2C Account Manager", MB_YESNO) == IDYES) {
			//toForce = session_generator::generate_session();
		//}
	//}

	//Lets apply our hooks
	ilog("Applying hooks...");
	hook_manager::apply_all();
	ilog("Hooks applied");

	//PLH::x64Detour* cfwDetour = new PLH::x64Detour((uint64_t)CreateFileW, (uint64_t)CreateFileWCallback, &oCreateFileW);
	//cfwDetour->hook();

	//If we are building with CHEAT_ONLY we dont want the extra windows
#if CHEAT_ONLY == 0
	render_mgr::registerListener(new decryptor());
	render_mgr::registerListener(new player_info());
	render_mgr::registerListener(new js_console());
	//render_mgr::registerListener(new redshift_display());
	render_mgr::registerListener(new session_generator());
	render_mgr::registerListener(new inject_playground());
	render_mgr::registerListener(new win_bot());
	render_mgr::registerListener(new packet_viewer());
	render_mgr::registerListener(new transaction_viewer());
	render_mgr::registerListener(new request_viewer());
#endif
	render_mgr::registerListener(new cheat_menu());

	//Setup javascript
	//js_wrapper::setup_runtime();

	//Auth check
	auth::authInfo* userInfo = auth::readAuthData();
	if (!userInfo) { ALERTA("Fatal error... this is a bug! (Is your injector updated?)"); };
	if (!userInfo || !userInfo->isValid()) {
		throw std::exception("Can't find game_instance... did the base pointer change?");
		return 1;
	}

	//Start the anticheat bypass proxy
	//bfb_proxy::start_server();

	//Our initialization is done, the rest of the work is done on game threads
	return 0;
}

bool __stdcall DllMain(HINSTANCE dll_instance, DWORD reason, LPVOID lp_reserved) {
	//Check our DLL load reason
	if (reason == DLL_PROCESS_ATTACH) {
		//If its attached, start the initialization
		//start_cheat(dll_instance);
		ilog("Code is running");
		auto thread_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)start_cheat, dll_instance, 0, nullptr);
		ilog("Thread started");
	}
	return true;
}