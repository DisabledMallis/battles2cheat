#include "formdatahook.h"
#include "../../memory.h"
#include <logger.h>
#include "../../seshgen/session_generator.h"

extern bool bp3;

static uint64_t oFormData;
static auth_info* backupUser;

void f_formData(DGAnalytics_Redshift* redshift_data, nuv::string* userId, void* param_3, int param_4) {
	PLH::FnCast(oFormData, f_formData)(redshift_data, userId, param_3, param_4);
}

auto formdatahook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? 45 8B ?? ?? 89 ?? ?? 49");

	this->create_detour(fp, (uint64_t)&f_formData, (uint64_t*)&oFormData);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto formdatahook::remove() -> bool {
	return this->get_detour()->unHook();
}