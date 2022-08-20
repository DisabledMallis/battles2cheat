#ifndef SRC_BASE_SCREEN
#define SRC_BASE_SCREEN

#include <nuv.h>
#include "base_pointers.h"

class base_screen : public base_pointers
{
public:
	char pad_00D8[32]; //0x00D8
	class N00006293* N0000395D; //0x00F8
	char pad_0100[40]; //0x0100
	nuv::string screen_name; //0x0128
	char pad_0148[72]; //0x0148
	class N0000647A* N0000396C; //0x0190
	class CGameSystemPointers* game_pointers; //0x0198
	class account_wrapper* account_info; //0x01A0
	char pad_01A8[192]; //0x01A8
}; //Size: 0x0268

#endif // !SRC_BASE_SCREEN