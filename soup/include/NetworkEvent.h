#pragma once

#include <stdint.h>

namespace NetworkEvent {
	class SNetPlaceTowerEvent
	{
	public:
		char pad_0000[8]; //0x0000
		int32_t N000066B5; //0x0008
		char pad_000C[4]; //0x000C
		int32_t N000066B6; //0x0010
		char pad_0014[12]; //0x0014
		int32_t N000066B8; //0x0020
		char pad_0024[4]; //0x0024
		int32_t type; //0x0028
		int32_t N000066D0; //0x002C
		int32_t placementCount; //0x0030
		char pad_0034[4]; //0x0034
		float posX; //0x0038
		float posY; //0x003C
		int32_t N000066BD; //0x0040
		float cost; //0x0044
		char pad_0048[16]; //0x0048
		int32_t N000066C0; //0x0058
		char pad_005C[36]; //0x005C
	}; //Size: 0x0080

	class SNetRequestQueueBloonSetEvent
	{
	public:
		char pad_0000[40]; //0x0000
		int64_t receiveType; //0x0028
		int32_t sendType; //0x0030
		bool leftSend; //0x0034
		char pad_0035[3]; //0x0035
	}; //Size: 0x0038

	class SNetEcoUpdatedEvent
	{
	public:
		char pad_0000[16]; //0x0000
		int32_t maybeTick; //0x0010
		char pad_0014[12]; //0x0014
		float cash; //0x0020
		float eco; //0x0024
		char pad_0028[40]; //0x0028
		float cash2; //0x0050
		float cash3; //0x0054
		char pad_0058[72]; //0x0058
	}; //Size: 0x00A0
};