#ifndef SRC_HOOKS_NETWORK_QUEUE_GET_RESPONSE_HOOK
#define SRC_HOOKS_NETWORK_QUEUE_GET_RESPONSE_HOOK

#include "../hook.h"
#include <network_queue.h>
#include <vector>

namespace network_queue {
	class getresponse_hook : public hook {
	public:
		getresponse_hook() : hook("get_response")
		{}
		auto apply() -> bool override;
		auto remove() -> bool override;
	};
};

#endif /* SRC_HOOKS_NETWORK_QUEUE_GET_RESPONSE_HOOK */