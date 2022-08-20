#pragma once

#include "../hook.h"

namespace NK {
	class getuseridhook : public hook {
	public:
		getuseridhook() : hook("GetUserID")
		{}
		auto apply() -> bool override;
		auto remove() -> bool override;
	};
}