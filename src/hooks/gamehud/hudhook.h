#ifndef SRC_HOOKS_GAMEHUD_HUDHOOK
#define SRC_HOOKS_GAMEHUD_HUDHOOK

#include "../hook.h"
#include <game_hud.h>

class hudhook : public hook {
public:
	hudhook() : hook("game hud") {};
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif