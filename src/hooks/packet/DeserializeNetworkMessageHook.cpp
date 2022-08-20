#include "DeserializeNetworkMessageHook.h"

#include "memory.h"
#include <reflang.h>
#include <NetworkEvent.h>
#include <CNetworkMessage.h>
#include <logger.h>
#include <game_hud.h>
#include <Game.h>

extern bool cancelSends;

extern CGameHudScreen* p_gamehud;


uint64_t oDeserializeNetworkMessage;
bool f_DeserializeNetworkMessage(CNetworkMessage* pCNetworkMessage, reflang::Class<void>* pRefClass, reflang::Reference<void>* pMessageRef) {

	std::string typeName = memory::ReadTypeName(pRefClass);
#ifdef _DEBUG
	ilog("DOWN - %s", typeName.c_str());
#endif

	if (cancelSends) {
		if (p_gamehud) {
			auto pGameScreen = p_gamehud->pGameScreen;
			if (pGameScreen) {
				auto pGameRunner = pGameScreen->pGameRunner;
				if (pGameRunner) {
					if (pGameRunner->serverAddress != "invalid_server_offline_only") {
						if (typeName.find("RequestQueueBloonSet") != std::string::npos) {
							ilog("Ignored send request");
							return false;
						}
					}
				}
			}
		}
	}

	return PLH::FnCast(oDeserializeNetworkMessage, f_DeserializeNetworkMessage)(pCNetworkMessage, pRefClass, pMessageRef);
}

bool DeserializeNetworkMessageHook::apply()
{
	//?? 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? 48 ?? ?? 48 8B ?? 48 8B
	uintptr_t fp = (uintptr_t)(memory::FindSig("89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 83 ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? 48 ?? ?? 48 8B ?? 48 8B")-1);

	this->create_detour(fp, (uint64_t)&f_DeserializeNetworkMessage, (uint64_t*)&oDeserializeNetworkMessage);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

bool DeserializeNetworkMessageHook::remove()
{
	return this->get_detour()->unHook();
}
