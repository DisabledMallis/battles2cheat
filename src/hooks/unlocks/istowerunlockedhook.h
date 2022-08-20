#ifndef SRC_HOOKS_UNLOCKS_ISTOWERUNLOCKEDHOOK
#define SRC_HOOKS_UNLOCKS_ISTOWERUNLOCKEDHOOK

#include "../hook.h"
#include <profile.h>

class istowerunlockedhook : public hook {
public:
	istowerunlockedhook() : hook("tower_unlocked")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_UNLOCKS_ISTOWERUNLOCKEDHOOK */
