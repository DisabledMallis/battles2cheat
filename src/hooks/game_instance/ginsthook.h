#ifndef SRC_HOOKS_GAME_INSTANCE_GINSTHOOK
#define SRC_HOOKS_GAME_INSTANCE_GINSTHOOK

#include "../hook.h"
#include <game_instance.h>

class ginsthook : public hook {
public:
	ginsthook() : hook("game_instance")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_GAME_INSTANCE_GINSTHOOK */
