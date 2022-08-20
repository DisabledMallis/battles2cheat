#include "gethsteamuser_hook.h"
#include <memory.h>

uint64_t ogethsteamuser;

int f_gethsteamuser_hook() {
	return 0;
	return PLH::FnCast(ogethsteamuser, f_gethsteamuser_hook)();
}

auto gethsteamuser_hook::apply() -> bool {
	HMODULE steam_api = GetModuleHandleA("steam_api64");
	uintptr_t fp = (uintptr_t)GetProcAddress(steam_api, "SteamAPI_GetHSteamUser");

	printf("SteamAPI_GetHSteamUser Address: %p", fp);

	this->create_detour(fp, (uint64_t)&f_gethsteamuser_hook, (uint64_t*)&ogethsteamuser);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto gethsteamuser_hook::remove() -> bool {
	return this->get_detour()->unHook();
}