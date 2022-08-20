#include "SNetRequestQueueBloonSetEventHook.h"
#include <NetworkEvent.h>
#include <memory.h>
#include <logger.h>

struct BloonData {
	char pad_0000[16]; //0x0000
	int32_t count; //0x0010
	char pad_0014[8]; //0x0014
	float cost; //0x001C
	float ecoBoost; //0x0020
	char pad_0024[8]; //0x0024
};

uint64_t oSNetRequestQueueBloonSetEvent;
extern float expectedCash;

NetworkEvent::SNetRequestQueueBloonSetEvent* hkSNetRequestQueueBloonSetEvent(NetworkEvent::SNetRequestQueueBloonSetEvent* self, BloonData* bloonData, char param_2, char param_3) {
	expectedCash -= bloonData->cost;
#ifdef _DEBUG
	ilog("Bloon sent, lowering expected cash to: %.2f", expectedCash);
#endif
	return PLH::FnCast(oSNetRequestQueueBloonSetEvent, hkSNetRequestQueueBloonSetEvent)(self, bloonData, param_2, param_3);
}

auto SNetRequestQueueBloonSetEventHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("45 33 ?? B8 71");

	this->create_detour(fp, (uint64_t)&hkSNetRequestQueueBloonSetEvent, (uint64_t*)&oSNetRequestQueueBloonSetEvent);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto SNetRequestQueueBloonSetEventHook::remove() -> bool {
	return this->get_detour()->unHook();
}