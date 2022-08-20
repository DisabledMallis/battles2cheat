#include "transaction_viewer.h"
#include <vector>

bool snapshot_transactions;

std::vector<TransactionSnapshot*> snapshots;
void onTransaction(LiNK::NKModuleStorage* module_storage, transactionData* transaction_data, void* param_2) {
	/*
	if (snapshot_transactions) {
		TransactionSnapshot* transaction_snapshot = new TransactionSnapshot();
		transaction_snapshot->uuid = transaction_data->uuid.c_str();
		for (auto trans : transaction_data->transactions) {
			TransDataSnapshot clone;
			clone.type = trans.type.c_str();
			clone.path = trans.path.c_str();
			clone.val_type = trans.val_type;
			memcpy(clone.value, trans.value, 32);
			if (trans.val_type == transaction_type::STRING) {
				char* value_str = (char*)malloc(2048);
				if (value_str) {
					strcpy_s(value_str, 2048, reinterpret_cast<nuv::string*>(&trans.value)->c_str());
					(*(char**)(&clone.value)) = value_str;
				}
			}
			clone.metadata = trans.metadata.c_str();
			transaction_snapshot->transactions.push_back(clone);
		}
		transaction_snapshot->filename = transaction_data->filename.c_str();
		transaction_snapshot->reason = transaction_data->reason.c_str();
		transaction_snapshot->transaction_key = transaction_data->transaction_key.c_str();
		snapshots.push_back(transaction_snapshot);
	}
	*/
}

transaction_viewer::transaction_viewer() : renderable::renderable("transaction_viewer")
{

}

void transaction_viewer::on_render()
{
	ImGui::Begin("Transaction Viewer");
	ImGui::Checkbox("snapshot transactions", &snapshot_transactions);
	ImGui::Text("Transaction Snapshots");
	ImGui::Separator();
	for (auto snapshot : snapshots) {
		ImGui::PushID(snapshot);
		char header_title[64];
		sprintf(header_title, "Request %s", snapshot->uuid.c_str());
		if (ImGui::CollapsingHeader(header_title)) {
			ImGui::Text("UUID: %s", snapshot->uuid.c_str());
			ImGui::Text("Transactions");
			for (auto trans : snapshot->transactions) {
				ImGui::Text("Type: %s", trans.type.c_str());
				ImGui::Text("Path: %s", trans.path.c_str());
				ImGui::Text("Value Type: %x", trans.val_type);
				switch (trans.val_type) {
				case transaction_type::STRING: {
					ImGui::Text("Value: %s", *reinterpret_cast<char**>(&trans.value));
					break;
				}
				case transaction_type::LONG:
				case transaction_type::BOOL:
				case transaction_type::INT: {
					ImGui::Text("Value: %x", *reinterpret_cast<int*>(&trans.value));
					break;
				}
				default: {
					ImGui::Text("Value: %p", &trans.value);
				}
				}
			}
			ImGui::Text("Filename: %s", snapshot->filename.c_str());
			ImGui::Text("Reason: %s", snapshot->reason.c_str());
			ImGui::Text("Transaction Key: %s", snapshot->transaction_key.c_str());
		}
		ImGui::PopID();
	}
	ImGui::End();
}
