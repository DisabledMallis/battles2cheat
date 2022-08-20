#include "LogTowerEventHook.h"
#include <stdint.h>
#include <memory.h>
#include <nuv.h>
#include <logger.h>
#include <game_hud.h>
#include <Simulation.h>
#include <internal.h>

extern bool bp3;
extern bool doubleEco;
extern float ecoSkip;

#define STARTING_CASH 650

static uint64_t oLogTowerEvent;
float expectedCash = STARTING_CASH;
static bool ecoTicking = false;
static float lNextPay = 0;
extern CGameHudScreen* p_gamehud;
extern player* localPlayer;
extern Simulation::Components::SPlayerComponent* localPlayerData;

void f_LogTowerEvent(nuv::string* towerName, int32_t towerSlot, int64_t purchaseCost, int64_t cashBefore, int64_t cashAfter, int64_t eco, char upgradePathA, char upgradePathB, char upgradePathC) {
	if (bp3) {
		if (upgradePathA > 2) { return; }
		if (upgradePathB > 2) { return; }
		if (upgradePathC > 2) { return; }

		bool foundName = false;
		for (int i = 0; i < (sizeof(defaultTowerNames)/sizeof(char*)); i++) {
			if (towerName->getString() == defaultTowerNames[i]) {
				foundName = true;
				break;
			}
		}
		if (!foundName) {
			return;
		}

		//Set the before cash
		cashBefore = expectedCash;
		//We spent some cash, so remove it
		expectedCash -= purchaseCost;
		if (expectedCash < 0) {
			//Calculate the amount of money we just pulled out of our ass
			ilog("Unexpected cash increase of: %.2f", abs(expectedCash));
			//No negative expected cash
			expectedCash = 0;
		}

		cashAfter = (cashBefore - purchaseCost);
		if (cashAfter < 0) {
			cashBefore += abs(cashAfter);
			cashAfter = 0;
		}
	}
#ifdef _DEBUG
	ilog(LogLevel::Warning, "Tower Log Event Details");
	ilog("towerName: %s", towerName->c_str());
	ilog("towerSlot: %d (%x)", towerSlot, towerSlot);
	ilog("purchaseCost: %d (%x)", purchaseCost, purchaseCost);
	ilog("cashBefore: %d (%x)", cashBefore, cashBefore);
	ilog("cashAfter: %d (%x)", cashAfter, cashAfter);
	ilog("eco: %d (%x)", eco, eco);
	ilog("upgradePathA: %d (%x)", upgradePathA, upgradePathA);
	ilog("upgradePathB: %d (%x)", upgradePathB, upgradePathB);
	ilog("upgradePathC: %d (%x)", upgradePathC, upgradePathC);
#endif

	PLH::FnCast(oLogTowerEvent, f_LogTowerEvent)(towerName, towerSlot, purchaseCost, cashBefore, cashAfter, eco, upgradePathA, upgradePathB, upgradePathC);
}

auto LogTowerEventHook::apply() -> bool {
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 4D 8B ?? 4D 8B ?? 44");

	std::thread calcThread([]() {
		while (true) {

			if (localPlayerData) {
				float cash = localPlayerData->cash.decrypt_value();
				float nextPay = localPlayerData->next_pay.decrypt_value();
				if (nextPay != lNextPay) {
					ecoTicking = true;
					lNextPay = nextPay;
				}
				else {
					ecoTicking = false;
				}

				//If our actual cash is lower, its probably safer to use that amount
				if (cash < expectedCash) {
					expectedCash = cash;
#ifdef _DEBUG
					ilog("Cash was less than whats expected, so the expected value was set to cash");
#endif
				}
			}
			else {
				ecoTicking = false;
				expectedCash = STARTING_CASH;
			}

			if (ecoTicking) {
				Sleep(6 * 1000);
				if (localPlayerData) {
					expectedCash += localPlayerData->eco.decrypt_value();
				}
#ifdef _DEBUG
				ilog("Expected eco tick");
				ilog("Expected cash: %.2f", expectedCash);
#endif
			}
			else {
				Sleep(10);
				continue;
			}
		}
	});
	calcThread.detach();

	this->create_detour(fp, (uint64_t)&f_LogTowerEvent, (uint64_t*)&oLogTowerEvent);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto LogTowerEventHook::remove() -> bool {
	return this->get_detour()->unHook();
}