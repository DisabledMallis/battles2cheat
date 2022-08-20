#ifndef SRC_HOOKS_GAMESCREEN_LOADHOOK
#define SRC_HOOKS_GAMESCREEN_LOADHOOK

#include "../hook.h"

class gscreen_load : public hook {
public:
	gscreen_load() : hook("game screen") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif