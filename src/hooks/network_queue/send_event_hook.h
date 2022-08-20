#ifndef SRC_HOOKS_NETWORK_QUEUE_SEND_EVENT_HOOK
#define SRC_HOOKS_NETWORK_QUEUE_SEND_EVENT_HOOK

#include "../hook.h"
#include <network_queue.h>
#include <vector>

class sendevent_hook : public hook {
public:
	sendevent_hook() : hook("send_event")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_NETWORK_QUEUE_SEND_EVENT_HOOK */