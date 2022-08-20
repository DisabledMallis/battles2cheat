#include "getdeviceidhook.h"
#include <memory.h>
#include "../../seshgen/session_generator.h"
#include <logger.h>
#include <nuv.h>

static uint64_t oGetDeviceID;

extern bool bp3;

char randChar() {
	if(rand() % 2 == 0)
		return rand() % ('z' - 'a' + 1) + 'a';
	else
		return rand() % ('9' - '0' + 1) + '0';
}

#define ID_LEN 32
static char newId[ID_LEN];

nuv::string* hkGetDeviceID(void* something) {
	nuv::string* deviceId = PLH::FnCast(oGetDeviceID, hkGetDeviceID)(something);
	for (int i = 1; i <= ID_LEN; i++) {
		deviceId->at(i) = newId[i-1];
	}
	return deviceId;
}

auto NK::getdeviceidhook::apply() -> bool {
	// Get the system time.
	unsigned seed = time(0);
	// Seed the random number generator.
	srand(seed);

	for (int i = 0; i < ID_LEN; i++) {
		newId[i] = randChar();
	}

	uintptr_t fp = (uintptr_t)memory::FindSig("?? 89 ?? ?? ?? ?? 89 ?? ?? ?? ?? 89 ?? ?? ?? 55 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 4C 8B ?? ?? 89 ?? ?? ?? FF");

	this->create_detour(fp, (uint64_t)&hkGetDeviceID, (uint64_t*)&oGetDeviceID);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto NK::getdeviceidhook::remove() -> bool {
	return this->get_detour()->unHook();
}