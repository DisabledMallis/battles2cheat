#include "lscreen_load.h"
#include <memory.h>
#include <mutex>
#include <CLobbyScreen.h>
#include <logger.h>

static uint64_t ogsloadfunc;

void* lscreen_load_hook(CLobbyScreen* self, size_t param_1, size_t param_2, nuv::string param_3) {

	//ilog("CLobbyScreen loaded, re activate bypass!");

	return PLH::FnCast(ogsloadfunc, lscreen_load_hook)(self, param_1, param_2, param_3);
}

auto lscreen_load::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 55 56 57 41 ?? 48 83 ?? ?? 49 8B ?? 49 8B ?? 48");

	this->create_detour(fp, (uint64_t)&lscreen_load_hook, (uint64_t*)&ogsloadfunc);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto lscreen_load::remove() -> bool {
	return this->get_detour()->unHook();
}