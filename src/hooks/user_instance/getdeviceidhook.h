#pragma once

#include "../hook.h"

namespace NK {
	class getdeviceidhook : public hook {
	public:
		getdeviceidhook() : hook("GetDeviceID")
		{}
		auto apply() -> bool override;
		auto remove() -> bool override;
	};
}