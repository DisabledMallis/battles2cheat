#ifndef SRC_HOOKS_UNLOCKS_ISUPGRADEUNLOCKEDHOOK
#define SRC_HOOKS_UNLOCKS_ISUPGRADEUNLOCKEDHOOK

#include "../hook.h"
#include <profile.h>

class isupgradeunlockedhook : public hook {
public:
	isupgradeunlockedhook() : hook("upgrade_unlocked")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_UNLOCKS_ISUPGRADEUNLOCKEDHOOK */
