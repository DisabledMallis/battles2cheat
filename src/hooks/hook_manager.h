#ifndef HOOKS_HOOK_MANAGER
#define HOOKS_HOOK_MANAGER

#include <vector>

class hook_manager {
	static inline std::vector<class hook*>* hooks = new std::vector<class hook*>();
public:
	static void apply_all();
	static void apply_hook(class hook* toAdd);
};

#endif /* HOOKS_HOOK_MANAGER */
