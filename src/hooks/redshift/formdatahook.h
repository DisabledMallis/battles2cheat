#pragma once

#include "../hook.h"
#include <redshift.h>

class formdatahook : public hook {
public:
	formdatahook() : hook("redshift")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};