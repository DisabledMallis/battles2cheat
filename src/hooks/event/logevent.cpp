#include "logevent.h"
#include "../../memory.h"
#include <logger.h>
#include "../../seshgen/session_generator.h"
#include <game_hud.h>
#include <Simulation.h>
#include <stdint.h>

extern bool bp3;

static uint64_t oLogEvent;

void f_logEvent(char* param_1, int64_t param_2, char param_3, int32_t param_4, int32_t param_5) {
	PLH::FnCast(oLogEvent, f_logEvent)(param_1, param_2, param_3, param_4, param_5);
}

auto LogEventHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 81 ?? ?? ?? ?? ?? 45 8B ?? 41");

	//backupUser = session_generator::generate_session();

	this->create_detour(fp, (uint64_t)&f_logEvent, (uint64_t*)&oLogEvent);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto LogEventHook::remove() -> bool {
	return this->get_detour()->unHook();
}