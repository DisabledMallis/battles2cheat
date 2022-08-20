#ifndef SRC_HOOKS_STEAMCLIENT_GETUSERID_HOOK
#define SRC_HOOKS_STEAMCLIENT_GETUSERID_HOOK

#include "../hook.h"

namespace Steam {
	class getuseridhook : public hook {
	public:
		getuseridhook() : hook("getuserid")
		{}
		auto apply() -> bool override;
		auto remove() -> bool override;
	};
}

#endif /* SRC_HOOKS_STEAMCLIENT_GETUSERID_HOOK */
