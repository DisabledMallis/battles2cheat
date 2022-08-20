#include "send_request_hook.h"
#include <memory.h>
#include "../../network/request_viewer.h"
#include <json/json.hpp>

static uint64_t ogsendrequest;

void* f_sendrequesthook(LiNK::NKModuleUser* module_user, nuv::string* api_endpoint, nuv::string* request_data, int param_3, void* param_4, void* param_5, int param_6, int param_7, int param_8) {
	return PLH::FnCast(ogsendrequest, f_sendrequesthook)(module_user, api_endpoint, request_data, param_3, param_4, param_5, param_6, param_7, param_8);
}

auto module_user::sendrequest_hook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 8D ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? 48 89 ?? ?? 41 8B ?? 4D 8B");

	this->create_detour(fp, (uint64_t)&f_sendrequesthook, (uint64_t*)&ogsendrequest);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto module_user::sendrequest_hook::remove() -> bool {
	return this->get_detour()->unHook();
}