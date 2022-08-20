#include "setfloathook.h"
#include <enc_val.h>
#include <memory.h>
#include <Simulation.h>

uint64_t ogsetfloat;
extern bool godmode;
extern bool gmValLock;

extern bool reduceDamage;
extern float reducePercent;

extern Simulation::Components::SPlayerComponent* localPlayerData;

void* f_setFloatHook(enc_val_float* pEncVal, float* value) {

	if (godmode) {
		if (gmValLock) {
			if (localPlayerData) {
				if (&localPlayerData->health == pEncVal) {
					*value = 150.0f;
				}
			}
		}
	}
	if (reduceDamage && reducePercent > 0) {
		if (localPlayerData) {
			if (&localPlayerData->health == pEncVal) {
				float healthBefore = localPlayerData->health.decrypt_value();
				float healthAfter = *value;
				if (healthBefore > healthAfter) {
					float diff = healthBefore - healthAfter;
					float newReduction = diff - (diff * (reducePercent/100));
					*value = healthBefore - newReduction;
				}
			}
		}
	}

	return PLH::FnCast(ogsetfloat, f_setFloatHook)(pEncVal, value);
}

auto setFloatHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig(SIG_FLOAT_SET);

	this->create_detour(fp, (uint64_t)&f_setFloatHook, (uint64_t*)&ogsetfloat);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto setFloatHook::remove() -> bool {
	return this->get_detour()->unHook();
}