#ifndef SRC_GAME_INSTANCE
#define SRC_GAME_INSTANCE

#include "base_pointers.h"

class game_instance
{
public:
	size_t client_uuid; //0x0000
	char pad_0008[96]; //0x0008
	void* WinFileIO; //0x0068
	base_pointers* CGameSystemPointers; //0x0070
	char pad_0078[40]; //0x0078
	void* WinFileIO_2; //0x00A0
	char pad_00A8[2848]; //0x00A8
}; //Size: 0x0BC8

#endif /* SRC_GAME_INSTANCE */
