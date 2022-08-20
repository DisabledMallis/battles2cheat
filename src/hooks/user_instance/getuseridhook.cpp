#include "getuseridhook.h"
#include <memory.h>
#include "../../seshgen/session_generator.h"
#include <logger.h>
#include <nuv.h>

static uint64_t oGetUserID;

extern bool bp3;
static std::string oUserID;

nuv::string* hkGetUserID(void* userInfo, nuv::string* resultId) {
	return PLH::FnCast(oGetUserID, hkGetUserID)(userInfo, resultId);
}

auto NK::getuseridhook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? ?? 89 ?? ?? ?? 57 48 83 ?? ?? 48 8B ?? 48 8B ?? ?? 89 ?? ?? ?? ?? 44");

	this->create_detour(fp, (uint64_t)&hkGetUserID, (uint64_t*)&oGetUserID);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto NK::getuseridhook::remove() -> bool {
	return this->get_detour()->unHook();
}

//Could be a way to unban

//48 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 48 8B ?? 48 83 EC ?? 4D 8B ?? 48 8B ?? 48 8B ?? E8