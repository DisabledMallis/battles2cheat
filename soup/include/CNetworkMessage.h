#pragma once

class CNetworkMessage
{
public:
	char pad_0000[32]; //0x0000
	int64_t count; //0x0020
	int64_t messageId; //0x0028
	char pad_0030[32]; //0x0030
}; //Size: 0x0050