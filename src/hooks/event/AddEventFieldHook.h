#pragma once

#include "../hook.h"

class AddEventFieldHook : public hook {
public:
	AddEventFieldHook() : hook("AddEventField") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};