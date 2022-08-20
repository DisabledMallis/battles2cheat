#ifndef SRC_NETWORK_TRANSACTION_VIEWER
#define SRC_NETWORK_TRANSACTION_VIEWER

#include "../render/renderable.h"
#include <transaction.h>
#include <NKModule.h>

/*
union TransactionValue {
	nuv::string s_value;
	int i_value;
	void* u_value;
};
*/
struct TransDataSnapshot {
	std::string type;
	std::string path;
	transaction_type val_type;
	char value[32];
	std::string metadata;

	TransDataSnapshot() {
		this->type = "";
		this->path = "";
		this->metadata = "";
	}
};
struct TransactionSnapshot {
	std::string uuid;
	std::vector<TransDataSnapshot> transactions;
	std::string filename;
	std::string reason;
	std::string transaction_key;
};

void onTransaction(LiNK::NKModuleStorage* module_storage, transactionData* transaction_data, void* param_2);

struct transaction_viewer : public renderable {
	transaction_viewer();
	void on_render() override;
};

#endif /* SRC_NETWORK_TRANSACTION_VIEWER */
