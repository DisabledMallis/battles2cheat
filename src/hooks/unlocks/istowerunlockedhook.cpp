#include "istowerunlockedhook.h"
#include <memory.h>

uint64_t ogistowerunlocked;
extern bool unlockAllTowers;

bool f_isTowerUnlocked(size_t param_1, int param_2) {
	if (unlockAllTowers) {
		return true;
	}
	return PLH::FnCast(ogistowerunlocked, f_isTowerUnlocked)(param_1, param_2);
}

auto istowerunlockedhook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("48 83 ?? ?? 48 81 ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 85 ?? 74 ?? 0F");

	this->create_detour(fp, (uint64_t)&f_isTowerUnlocked, (uint64_t*)&ogistowerunlocked);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto istowerunlockedhook::remove() -> bool {
	return this->get_detour()->unHook();
}