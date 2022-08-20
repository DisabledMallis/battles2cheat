#ifndef SRC_CGAMESCREEN
#define SRC_CGAMESCREEN

#include "base_screen.h"
#include "Scene.h"
#include "Game.h"
#include "Assets.h"

class CGameScreen : public base_screen
{
public:
	char pad_0268[1968]; //0x0268
	nuv::string someSoupareaIp; //0x0A18
	nuv::string someCode; //0x0A38
	char pad_0A58[8232]; //0x0A58
	class N0000516F* N00004FEE; //0x2A80
	class Scene::Empty* pSceneRoot; //0x2A88
	class Assets::Asset(*pSceneAssets)[99]; //0x2A90
	char pad_2A98[11024]; //0x2A98
	class Game::GameRunner* pGameRunner; //0x55A8
	char pad_55B0[2384]; //0x55B0
}; //Size: 0x5F00

#endif