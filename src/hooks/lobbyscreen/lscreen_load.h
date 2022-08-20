#ifndef SRC_HOOKS_LOBBYSCREEN_LOADHOOK
#define SRC_HOOKS_LOBBYSCREEN_LOADHOOK

#include "../hook.h"

class lscreen_load : public hook {
public:
	lscreen_load() : hook("lobby screen") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif