#include "LogHeroChangeHook.h"
#include <stdint.h>
#include <memory.h>
#include <nuv.h>
#include <logger.h>
#include <game_hud.h>
#include <Simulation.h>

extern bool bp3;

static uint64_t oLogHeroChange;

void f_LogHeroChange(uint64_t param_1, nuv::string* prevHero, nuv::string* nextHero) {
	if (bp3) {
		//*nextHero = "Quincy";
		return;
	}
	return PLH::FnCast(oLogHeroChange, f_LogHeroChange)(param_1, prevHero, nextHero);
}

auto LogHeroChangeHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 4D 8B ?? 4C 8B ?? 48 8B ?? 48 8D ?? ?? ?? 89 ?? ?? 48 ?? ?? ?? ?? 89 ?? ?? 48 ?? ?? ?? ?? 89 ?? ?? 48 ?? ?? ?? ?? 89 ?? ?? 0F 57 ?? 33 C0 ?? 11 ?? ?? ?? ?? 11 ?? ?? ?? ?? 11 ?? ?? ?? ?? 11 ?? ?? ?? ?? 11 ?? ?? ?? 89 ?? ?? 8B 0D ?? ?? ?? ?? 65 ?? ?? ?? ?? ?? ?? ?? ?? 41 BD ?? ?? ?? ?? 48 ?? ?? ?? 42 ?? ?? ?? 39 05 ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 74 ?? 80 35 ?? ?? ?? ?? ?? 80 35 ?? ?? ?? ?? ?? 80 35 ?? ?? ?? ?? ?? 80 35 ?? ?? ?? ?? ?? 80 35 ?? ?? ?? ?? ?? F6");
	this->create_detour(fp, (uint64_t)&f_LogHeroChange, (uint64_t*)&oLogHeroChange);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto LogHeroChangeHook::remove() -> bool {
	return this->get_detour()->unHook();
}