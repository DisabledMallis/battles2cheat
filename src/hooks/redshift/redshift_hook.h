#ifndef SRC_HOOKS_REDSHIFT_REDSHIFT_HOOK
#define SRC_HOOKS_REDSHIFT_REDSHIFT_HOOK

#include "../hook.h"
#include <redshift.h>

class redshift_hook : public hook {
public:
	redshift_hook() : hook("redshift")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_REDSHIFT_REDSHIFT_HOOK */