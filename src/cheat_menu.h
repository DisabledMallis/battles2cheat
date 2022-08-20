#ifndef SRC_CHEAT_MENU
#define SRC_CHEAT_MENU

#include "render/renderable.h"
#include "hooks/hook.h"
#include "player.h"

struct cheat_menu : public renderable, public hook {
	cheat_menu();
	void on_render() override;
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_CHEAT_MENU */
