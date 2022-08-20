#ifndef SRC_HOOKS_MODULE_USER_SEND_REQUEST_HOOK
#define SRC_HOOKS_MODULE_USER_SEND_REQUEST_HOOK

#include "../hook.h"
#include <network_queue.h>
#include <vector>

namespace module_user {
	class sendrequest_hook : public hook {
	public:
		sendrequest_hook() : hook("send_request")
		{}
		auto apply() -> bool override;
		auto remove() -> bool override;
	};
};

#endif /* SRC_HOOKS_MODULE_USER_SEND_REQUEST_HOOK */