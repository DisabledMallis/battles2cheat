#ifndef SRC_HOOKS_MODULE_STORAGE_SERIALIZE_HOOK
#define SRC_HOOKS_MODULE_STORAGE_SERIALIZE_HOOK

#include "../hook.h"
#include <network_queue.h>
#include <transaction.h>
#include <vector>

class SendTransactionsHook : public hook {
public:
	SendTransactionsHook() : hook("NKModuleStorage::SendTransactions") {}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_MODULE_STORAGE_SERIALIZE_HOOK */