#ifndef SRC_HOOKS_GAMEHUD_HUD_DESTRUCTOR
#define SRC_HOOKS_GAMEHUD_HUD_DESTRUCTOR

#include "../hook.h"
class hud_destructor : public hook {
public:
	hud_destructor() : hook("hud destructor") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif