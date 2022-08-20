#ifndef SRC_HOOKS_BALANCES_HOOK_BALANCES_HOOK
#define SRC_HOOKS_BALANCES_HOOK_BALANCES_HOOK

#include "../hook.h"

class balances_hook : public hook {
public:
	balances_hook() : hook::hook("pull_balances") {}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif