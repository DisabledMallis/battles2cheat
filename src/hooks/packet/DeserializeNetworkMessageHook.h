#pragma once

#include "../hook.h"

class DeserializeNetworkMessageHook : public hook {
public:
	DeserializeNetworkMessageHook() : hook::hook("DeserializeNetworkMessage") {};
	bool apply() override;
	bool remove() override;
};