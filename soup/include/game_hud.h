#ifndef SRC_GAME_HUD
#define SRC_GAME_HUD

#include "CGameScreen.h"
#include <stdint.h>
#include "maths.h"

class SGameScreenController
{
public:
	char pad_0000[352]; //0x0000
	int32_t gameTick; //0x0160
	char pad_0164[4]; //0x0164
	float N00002C54; //0x0168
	float N00006485; //0x016C
	char pad_0170[640]; //0x0170
}; //Size: 0x03F0

class bloon_send_data
{
public:
	float bloon_cost; //0x0000
	char pad_0004[12]; //0x0004
	float eco_boost; //0x0010
	char pad_0014[864]; //0x0014
}; //Size: 0x0374

class bloon_send_button
{
public:
	char pad_0000[8]; //0x0000
	class bloon_send_data* bloon_data; //0x0008
	int32_t bloon_type; //0x0010
	int16_t modifier; //0x0014
	char pad_0016[58]; //0x0016
	int32_t button_slot; //0x0050
	char pad_0054[12]; //0x0054
}; //Size: 0x0060

#define BLOON_SEND_BTN_CT 11
#define BLOON_SLOT_START 1
class bloon_button_list
{
public:
	bloon_send_button bloon_buttons[BLOON_SEND_BTN_CT];
};

class CGameHudScreen : public base_screen
{
public:
	char pad_0268[232]; //0x0268
	class SGameScreenController* pSGameScreenController; //0x0350
	char pad_0358[824]; //0x0358
	class CGameScreen* pGameScreen; //0x0690
	char pad_0698[40]; //0x0698
	class N00003A09* scene; //0x06C0
	char pad_06C8[1680]; //0x06C8
	class bloon_send_button(*bloon_buttons)[11]; //0x0D58
	void* bloon_btn_stop_at; //0x0D60
	char pad_0D68[504]; //0x0D68
	class SGameScreenController* pGameScreenController; //0x0F60
	char pad_0F68[184]; //0x0F68
	class generic_ui_controller* (*tower_buttons)[4]; //0x1020
	char pad_1028[2848]; //0x1028
}; //Size: 0x1EA0

#endif