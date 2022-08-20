#ifndef SRC_HOOKS_USER_INSTANCE_USERHOOK
#define SRC_HOOKS_USER_INSTANCE_USERHOOK

#include "../hook.h"
#include <profile.h>

class userhook : public hook {
public:
	userhook() : hook("user_instance")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_USER_INSTANCE_USERHOOK */
