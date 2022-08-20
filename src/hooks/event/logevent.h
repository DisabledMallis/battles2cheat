#pragma once

#include "../hook.h"
#include <redshift.h>

class LogEventHook : public hook {
public:
	LogEventHook() : hook("logevent")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};