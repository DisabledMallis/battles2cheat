#include "gscreen_load.h"
#include <memory.h>
#include <mutex>

static uint64_t ogsloadfunc;
std::mutex load_locker;

void(*gscr_pre_lock)() = nullptr;
void(*gscr_post_lock)() = nullptr;
void(*gscr_unlock)() = nullptr;

void* gscreen_load_hook(uintptr_t param_1, uintptr_t screen_loader , uintptr_t param_3, uintptr_t param_4, uintptr_t param_5, uintptr_t param_6, uintptr_t param_7, uintptr_t pp_hud, uintptr_t param_9, uintptr_t param_10, uintptr_t param_11, uintptr_t param_12, uintptr_t param_13) {
	if (gscr_pre_lock != nullptr) {
		gscr_pre_lock();
	}
	load_locker.lock();
	if (gscr_post_lock != nullptr) {
		gscr_post_lock();
	}
	load_locker.unlock();
	if (gscr_unlock != nullptr) {
		gscr_unlock();
	}
	return PLH::FnCast(ogsloadfunc, gscreen_load_hook)(param_1, screen_loader, param_3, param_4, param_5, param_6, param_7, pp_hud, param_9, param_10, param_11, param_12, param_13);
}

auto gscreen_load::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? B8 18");

	this->create_detour(fp, (uint64_t)&gscreen_load_hook, (uint64_t*)&ogsloadfunc);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto gscreen_load::remove() -> bool {
	return this->get_detour()->unHook();
}