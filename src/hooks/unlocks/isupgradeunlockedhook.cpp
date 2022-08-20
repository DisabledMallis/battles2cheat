#include "isupgradeunlockedhook.h"
#include <memory.h>

uint64_t ogisupgradeunlocked;
extern bool unlockAllUpgrades;

bool f_isUpgradeUnlocked(size_t param_1, size_t param_2, int param_3, int param_4) {
	if (unlockAllUpgrades) {
		return true;
	}
	return PLH::FnCast(ogisupgradeunlocked, f_isUpgradeUnlocked)(param_1, param_2, param_3, param_4);
}

auto isupgradeunlockedhook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("89 ?? ?? ?? 57 48 83 ?? ?? 48 81 ?? ?? ?? ?? ?? 41")-1;

	this->create_detour(fp, (uint64_t)&f_isUpgradeUnlocked, (uint64_t*)&ogisupgradeunlocked);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto isupgradeunlockedhook::remove() -> bool {
	return this->get_detour()->unHook();
}