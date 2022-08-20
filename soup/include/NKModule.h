#ifndef SRC_MODULE_STORAGE
#define SRC_MODULE_STORAGE

#include "transaction.h"
#include "memory.h"
#include <nuv.h>

//The hooked function that sends the data
extern void* f_serializehook(void* module_storage, transactionData* transaction_data, void* param_2);

namespace LiNK {
	class NKModule {};
	class NKModuleUtility : public NKModule {};
	class NKModuleUser : public NKModule {};
	class NKModuleStorage : public NKModule {
	public:
		static NKModuleStorage* GetInstance() {
			NKModuleStorage* (*fun_nkms_get_inst)(void*) = (NKModuleStorage * (*)(void*))memory::FindSig("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? 48 8B ?? ?? 48 85 ?? 75 ?? 8D 48 ?? E8 ?? ?? ?? ?? 48 8B ?? 48 89 ?? ?? ?? 48 85 ?? 74 ?? 0F 57 ?? 33 C0 0F 11 ?? 48 89 ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 48 8D ?? ?? ?? ?? ?? 48 89 ?? EB ?? 33 DB 48 89 ?? ?? 48 8B ?? 48 8B ?? ?? ?? 48 83 C4 ?? 5F C3 CC F3 0F");
			if (fun_nkms_get_inst) {
				return fun_nkms_get_inst(nullptr);
			}
			return nullptr;
		}
		void SendTransactions(transactionData* transactions) {
			f_serializehook(this, transactions, 0);
		}
	};
	class NKModuleBank : public NKModule {};
	class NKModuleMatchmaking : public NKModule {};
	class NKModulePayment : public NKModule {};
	class NKModulePubsub : public NKModule {};

	class NKModuleManager : public NKModule {
	public:
		class LiNK::NKModuleUtility* pNKModuleUtility; //0x0000
		char pad_0008[8]; //0x0008
		class LiNK::NKModuleUser* pNKModuleUser; //0x0010
		LiNK::NKModuleStorage* pNKModuleStorage; //0x0018
		class LiNK::NKModuleBank* pNKModuleBank; //0x0020
		char pad_0028[16]; //0x0028
		class LiNK::NKModuleMatchmaking* pNKModuleMatchmaking; //0x0038
		class LiNK::NKModulePayment* pNKModulePayment; //0x0040
		class LiNK::NKModulePubsub* pNKModulePubsub; //0x0048
		char pad_0050[56]; //0x0050
	};

	class StorageServices {
	public:
		char pad_0000[16]; //0x0000
		NKModuleManager* pNKModuleManager; //0x0010
		char pad_0018[8]; //0x0018
		class SNKManagerBlackboard* pSNKManagerBlackboard; //0x0020
		char pad_0028[40]; //0x0028
		nuv::string skuSignature; //0x0050

		static StorageServices* GetInstance() {
			StorageServices* (*fun_ss_get_inst)() = (StorageServices * (*)())memory::FindSig("40 ?? 48 83 EC ?? 48 8B ?? ?? ?? ?? ?? 48 85 ?? 75 ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B ?? 48 89 ?? ?? ?? 48 85 ?? 74 ?? 33 D2 41 B8 ?? ?? ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 90 48 89 ?? ?? ?? ?? ?? 48 83 C4");
			if (fun_ss_get_inst) {
				return fun_ss_get_inst();
			}
			return nullptr;
		}
		NKModuleManager* GetNKModuleManager() {
			return this->pNKModuleManager;
		}
	};
};
#endif // !SRC_MODULE_STORAGE
