#include "userhook.h"
#include <memory.h>
#include "../../seshgen/session_generator.h"
#include <logger.h>

uint64_t oguser;

profile* user_profile;
auth_info* toForce=nullptr;

void* f_userhook(profile*** prof) {
	if (prof) {
		if (*prof) {
			if (**prof) {
				user_profile = **prof;
			}
		}
	}
	/*
	if (toForce) {
		user_profile->avatar = toForce->user.avatar;
		user_profile->clan = toForce->user.clan;
		user_profile->continent = toForce->user.continent;
		user_profile->country = toForce->user.country;
		user_profile->displayName = toForce->user.displayName;
		user_profile->nkapiID = toForce->user.nkapiID;
		user_profile->online = toForce->user.online;
		user_profile->onlineApp = toForce->user.onlineApp;
		//user_profile->providers[0] = toForce->user.providersAvailable[0];
		user_profile->safeName = toForce->user.safeName;
		user_profile->sessionID = toForce->session.sessionID;
		user_profile->shortcode = toForce->user.shortcode;
		//ilog("Forced login");
	}
	*/
	return PLH::FnCast(oguser, f_userhook)(prof);
}

auto userhook::apply() -> bool {
    uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? 57 48 83 ?? ?? 48 8B ?? 48 8B ?? 48 ?? ?? 48 85 ?? 75 ?? 8D");

	this->create_detour(fp, (uint64_t)&f_userhook, (uint64_t*)&oguser);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto userhook::remove() -> bool {
	return this->get_detour()->unHook();
}

//Could be a way to unban

//48 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 48 8B ?? 48 83 EC ?? 4D 8B ?? 48 8B ?? 48 8B ?? E8