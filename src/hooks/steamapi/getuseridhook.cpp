//40 53 48 83 EC 20 48 8B 49 08 48 8B DA 48 8B 01 FF 50 38 48 8B C3 48 83 C4 20 5B C3

#include "getuseridhook.h"
#include <memory.h>
#include <logger.h>

uint64_t ogetuserid;
extern bool bp2;
extern bool bp2fail;

int* f_getuseridhook(size_t steamUser, size_t callback) {
	if (bp2) {
		int yes[2] = {0,0};
		(*(size_t*)&yes[0]) = rand();
		return yes;
	}
	return PLH::FnCast(ogetuserid, f_getuseridhook)(steamUser, callback);
}

auto Steam::getuseridhook::apply() -> bool {
	HMODULE steamclient = GetModuleHandleA("steamclient64");
	size_t steamclient_end = memory::GetModuleEnd("steamclient64");

	size_t fp = memory::FindSig((size_t)steamclient, steamclient_end, "40 ?? 48 ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? 48 ?? ?? FF ?? ?? 48 ?? ?? 48 ?? ?? ?? 5B C3 CC CC CC CC 48 ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 45");

	if (!fp) {
		ilog(LogLevel::Error, "Cannot find steam GetUserID function! Steam bypass is not compatible!");
		bp2fail = true;
		return false;
	}

	this->create_detour(fp, (uint64_t)&f_getuseridhook, (uint64_t*)&ogetuserid);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto Steam::getuseridhook::remove() -> bool {
	return this->get_detour()->unHook();
}