#include "SerializeNetworkMessageHook.h"

#include "memory.h"
#include <reflang.h>
#include <NetworkEvent.h>
#include <CNetworkMessage.h>
#include <logger.h>
#include <game_hud.h>
#include <Game.h>
#include <deque>
#include <limits>

extern bool godmode;
extern bool gmCancel;

extern bool bp3;

extern CGameHudScreen* p_gamehud;

uint64_t oSerializeNetworkMessage;
bool f_serializeNetworkMessage(reflang::Class<void>* pRefClass, CNetworkMessage* pCNetworkMessage, reflang::Reference<void>* pMessageRef) {

	std::string typeName = memory::ReadTypeName(pRefClass);
#ifdef _DEBUG
	ilog("UP - %s", typeName.c_str());
#endif

	if (p_gamehud) {
		auto pGameScreen = p_gamehud->pGameScreen;
		if (pGameScreen) {
			auto pGameRunner = pGameScreen->pGameRunner;
			if (pGameRunner) {
				if (pGameRunner->serverAddress != "invalid_server_offline_only") {
					if (godmode) {
						if (gmCancel) {
							if ((typeName.find("LifeChanged") != std::string::npos) || (typeName.find("LifeLost") != std::string::npos)) {
								ilog("Cancel life lost");
								return false;
							}
						}
					}
				}
			}
		}
	}
	return PLH::FnCast(oSerializeNetworkMessage, f_serializeNetworkMessage)(pRefClass, pCNetworkMessage, pMessageRef);
}

bool SerializeNetworkMessageHook::apply()
{
	//40 ?? 55 56 57 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? 48 ?? ?? 49 8B ?? 48 8B
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 55 56 57 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? 48 ?? ?? 49 8B ?? 48 8B");

	this->create_detour(fp, (uint64_t)&f_serializeNetworkMessage, (uint64_t*)&oSerializeNetworkMessage);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

bool SerializeNetworkMessageHook::remove()
{
	return this->get_detour()->unHook();
}
