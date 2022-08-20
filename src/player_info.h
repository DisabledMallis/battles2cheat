#ifndef SRC_PLAYER_INFO
#define SRC_PLAYER_INFO

#include "render/renderable.h"
#include "hooks/hook.h"
#include "player.h"

struct player_info : public renderable, public hook {
	player_info();
	void on_render() override;
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_PLAYER_INFO */
