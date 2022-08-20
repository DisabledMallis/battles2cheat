#pragma once

#include "ecs.h"
#include "enc_val.h"
#include "internal.h"

namespace Simulation {
	namespace Properties {
		struct SPlayerProps {
		};
	};
	namespace Components {
		struct SPlayerAbilityComponent : public ecs::EcsComponent {
			char pad_0000[40]; //0x0000
			int32_t count; //0x0028
			bool idk; //0x002C
			bool ready; //0x002D
		};

		static const char SIG_GETCHILDBYID[] = "?? 89 ?? ?? ?? ?? 89 ?? ?? ?? ?? 89 ?? ?? ?? 57 41 ?? 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 44 8B ?? 48 ?? ?? ?? ?? ?? ?? 4C";
		struct SPlayerComponent : public ecs::EcsComponent
		{
		public:
			char pad_0000[48]; //0x0000
			class N00003938* N00000414; //0x0030
			char pad_0038[16]; //0x0038
			Properties::SPlayerProps* pSPlayerProps; //0x0048
			Properties::SPlayerProps* pSPlayerProps2; //0x0050
			enc_val_float cash; //0x0058
			enc_val_float eco; //0x0070
			enc_val_float health; //0x0088
			enc_val_float next_pay; //0x00A0
			char pad_00B8[72]; //0x00B8

			SPlayerAbilityComponent* GetTowerBoostComponent() {
				return this->GetChildByID<SPlayerAbilityComponent>(1);
			}

			SPlayerAbilityComponent* GetBloonBoostComponent() {
				return this->GetChildByID<SPlayerAbilityComponent>(2);
			}

			template<typename T>
			T* GetChildByID(int id) {
				return callFastcall<SIG_GETCHILDBYID, T*, SPlayerComponent*, int>(this, id);
			}
		}; //Size: 0x0100
	};
};