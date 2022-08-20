#pragma once

#include "../hook.h"

class SerializeNetworkMessageHook : public hook {
public:
	SerializeNetworkMessageHook() : hook::hook("SerializeNetworkMessage") {};
	bool apply() override;
	bool remove() override;
};