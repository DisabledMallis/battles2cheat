#include "send_event_hook.h"
#include <memory.h>
#include "../../network/packet_viewer.h"

uint64_t ogsendevent;

void* f_sendeventhook(CLegacyNetworkQueue* pNetworkQueue, CNetworkMessage* pNetworkMessage, SInputEvent** ppSInputEvent, void* param_4) {
	onPacket(pNetworkQueue, pNetworkMessage, ppSInputEvent, param_4);
	return PLH::FnCast(ogsendevent, f_sendeventhook)(pNetworkQueue, pNetworkMessage, ppSInputEvent, param_4);
}

auto sendevent_hook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 48 ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? 48 8B ?? 49");

	this->create_detour(fp, (uint64_t)&f_sendeventhook, (uint64_t*)&ogsendevent);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto sendevent_hook::remove() -> bool {
	return this->get_detour()->unHook();
}