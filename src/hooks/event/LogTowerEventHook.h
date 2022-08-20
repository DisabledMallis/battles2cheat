#pragma once

#include "../hook.h"

class LogTowerEventHook : public hook {
public:
	LogTowerEventHook() : hook("LogTowerEvent")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};