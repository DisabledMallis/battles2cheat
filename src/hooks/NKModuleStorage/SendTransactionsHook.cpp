#include "SendTransactionsHook.h"
#include <memory.h>
#include "../../network/transaction_viewer.h"
#include <NKModule.h>
#include <logger.h>
#include <json/json.hpp>
#include <internal.h>

uint64_t oSendTransactions;
LiNK::NKModuleStorage* pModuleStorage = nullptr;

void hkSendTransactions(LiNK::NKModuleStorage* module_storage, transactionData* transaction_data, void* param_2) {
	if (!pModuleStorage) {
		pModuleStorage = module_storage;
	}
#ifdef _DEBUG
	nuv::vector transactions = transaction_data->transactions;
	int modIndex = 0;
	std::vector<int> transToRemove;
	for (int i = 0; i < transactions.count(); i++) {
		transaction* trans = transactions[i];
		std::string sAction = trans->action.getString();
		std::string sPath = trans->path.getString();
		std::string tMeta = trans->metadata.getString();
		transaction_type valType = trans->val_type;
		if (sPath.find("$.tower_loadout") != std::string::npos) {
			if (valType == transaction_type::ARRAY) {
				nuv::string sVal = trans->value.string_value;
				nlohmann::ordered_json jValArr = nlohmann::ordered_json::parse(sVal.getString());
				if (!jValArr.is_array()) {
					ilog(LogLevel::Error, "Expected an array, but it was not (trans bypass)");
					continue;
				}
				if (jValArr.size() > 4) {
					ilog(LogLevel::Error, "Array size exceeded expected size of 4 (trans bypass failure)");
				}
				for (int i = 0; i < jValArr.size(); i++) {
					jValArr[i] = defaultTowerIds[i];
				}
				transaction_data->transactions[i]->value.string_value.setString(jValArr.dump());
				continue;
			}
		}
		if (sPath == "$.towers" && sAction == "ARRAY_INSERT") {
			nuv::string& val = trans->value.string_value;
			std::string sVal = val.getString();
			nlohmann::ordered_json valObj = nlohmann::ordered_json::parse(sVal);
			if (valObj["unlocked"] != true) {
				//transToRemove.push_back(i);
				if (tMeta.find("game") == std::string::npos) {
					continue;
				}
				std::string newtype = "";
				switch (modIndex) {
				case 3:
					newtype = "BombShooter";
					break;
				case 2:
					newtype = "TackShooter";
					break;
				case 1:
					newtype = "DartMonkey";
					break;
				case 0:
				default:
					newtype = "Quincy";
					break;
				}
				valObj["type"] = newtype;
				valObj["unlocked"] = true;
				trans->value.string_value.setString(valObj.dump());
				ilog("Prevented the game from updating a non unlocked tower on the server side");
				//modified.push_back(i);
				modIndex++;
			}
		}
	}
#endif
	PLH::FnCast(oSendTransactions, hkSendTransactions)(module_storage, transaction_data, param_2);
}

auto SendTransactionsHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("48 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 8D ?? ?? ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? 48 89 ?? ?? ?? ?? ?? 49 8B ?? 48 8B ?? 4C 8B ?? 4C 89 ?? ?? 45 33 ?? 4C 89 ?? ?? 48 8D ?? ?? E8 ?? ?? ?? ?? 48 8D ?? ?? FF 55 ?? 48 89 ?? ?? ?? ?? ?? 4C 8D ?? ?? 48 8D ?? ?? ?? ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 4C 89 ?? ?? ?? ?? ?? 48 C7 85 E0 01 00 00 ?? ?? ?? ?? 44 88 ?? ?? ?? ?? ?? 45 33 ?? 48 8D ?? ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 45 33 ?? 48 8D ?? ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? FF 55 ?? 84 C0 0F 85 ?? ?? ?? ?? 48 8D ?? ?? ?? E8 ?? ?? ?? ?? 48 8B ?? 49 BE ?? ?? ?? ?? ?? ?? ?? ?? 44 38 ?? ?? 74 ?? 45 8B ?? 66 0F ?? ?? ?? ?? 41 0F ?? ?? 80 E1 ?? C0 E1 ?? 49 8B ?? 48 D3 ?? 41 30 ?? ?? 49 FF ?? 49 83 F8 ?? 72 ?? C6 46 67 ?? 48 8D ?? ?? ?? E8 ?? ?? ?? ?? 48 8B ?? 80 78 52");

	this->create_detour(fp, (uint64_t)&hkSendTransactions, (uint64_t*)&oSendTransactions);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto SendTransactionsHook::remove() -> bool {
	return this->get_detour()->unHook();
}