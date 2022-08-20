#ifndef SOUP_GAME
#define SOUP_GAME

#include "player.h"
#include "nuv.h"
#include "Scheduling.h"

namespace Game {
	class SInputEventProcessor
	{
	public:
	}; //Size: 0x0008

	class GameRunner {
	public:
		char pad_0000[328]; //0x0000
		class Game::GameRunner* pGameRunner; //0x0148
		char pad_0150[40]; //0x0150
		class gameResultLambda* pGameResultCallback; //0x0178
		char pad_0180[184]; //0x0180
		float tickDelta; //0x0238
		float gameSpeed; //0x023C
		char pad_0240[32]; //0x0240
		class Game::SInputEventProcessor* pSInputProcessor; //0x0260
		char pad_0268[88]; //0x0268
		float tickDelta2; //0x02C0
		float gameSpeed2; //0x02C4
		char pad_02C8[32]; //0x02C8
		class Game::GameRunner* pGameRunner2; //0x02E8
		char pad_02F0[56]; //0x02F0
		player players[2]; //0x0328
		char pad_20C8[16]; //0x20C8
		nuv::string serverAddress; //0x20D8
		char pad_20F8[40]; //0x20F8
		nuv::string game_code; //0x2120
		char pad_2140[8]; //0x2140
		nuv::string playerID; //0x2148
		nuv::string game_code2; //0x2168
		char pad_2188[8]; //0x2188
		class CLegacyNetworkQueue* pCLegacyNetworkQueue; //0x2190
		int32_t seconds; //0x2198
		char pad_219C[4]; //0x219C
		float someTimer; //0x21A0
		char pad_21A4[12]; //0x21A4
		class Scheduling::CLocalGameScheduler* pCLocalGameScheduler; //0x21B0
		char pad_21B8[248]; //0x21B8
	};
};

#endif // !SOUP_GAME
