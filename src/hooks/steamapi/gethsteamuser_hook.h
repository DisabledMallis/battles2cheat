#ifndef SRC_HOOKS_STEAMAPI_GETHSTEAMUSER_HOOK
#define SRC_HOOKS_STEAMAPI_GETHSTEAMUSER_HOOK

#include "../hook.h"

class gethsteamuser_hook : public hook {
public:
	gethsteamuser_hook() : hook("gethsteamuser")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_STEAMAPI_GETHSTEAMUSER_HOOK */
