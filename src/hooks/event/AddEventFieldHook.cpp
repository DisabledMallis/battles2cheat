#include "AddEventFieldHook.h"
#include <nuv.h>
#include <logger.h>
#include <memory.h>

extern bool bp3;

static uint64_t oAddEventField;

void hkAddEventField(nuv::string* eventName, nuv::string* key, nuv::string* value) {
	if (bp3) {
		if (*eventName == "startGame") {
			if (*key == "hero") {
				*value = "Quincy";
			}
			if (*key == "tower_one") {
				*value = "DartMonkey";
			}
			if (*key == "tower_two") {
				*value = "TackShooter";
			}
			if (*key == "tower_three") {
				*value = "BombShooter";
			}
			ilog("Modified startGame event");
		}
	}
	return PLH::FnCast(oAddEventField, hkAddEventField)(eventName, key, value);
}

auto AddEventFieldHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? 57 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? 48 8B ?? 48 8B ?? ?? 89 ?? ?? ?? 0F");
	this->create_detour(fp, (uint64_t)&hkAddEventField, (uint64_t*)&oAddEventField);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto AddEventFieldHook::remove() -> bool {
	return this->get_detour()->unHook();
}