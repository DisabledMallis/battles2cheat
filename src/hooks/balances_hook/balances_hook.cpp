#include "balances_hook.h"
#include <memory.h>
#include <balances.h>
#include <json/json.hpp>
#include <BanCodes.h>
#include <logger.h>

uint64_t obalfunc;
BAN_CODE ban_status = BAN_CODE::NOT_BANNED;

BAN_CODE parseForBan(const nlohmann::json& parsed_currencies) {
	if (parsed_currencies.contains("0xLDR")) {
		return BAN_CODE::STEAM;
	}
	if (parsed_currencies.contains("0x0A")) {
		int statusCode = parsed_currencies["0x0A"];
		switch (statusCode) {
		case 0:
			return BAN_CODE::UNKNOWN;
		case 1:
		{
			return BAN_CODE::ANTICHEAT;
		}
		break;
		case 8:
		{
			return BAN_CODE::ANTICHEAT2;
		}
		break;
		};
	}
	if (parsed_currencies.contains("0xBB")) {
		int statusCode = parsed_currencies["0xBB"];
		if (statusCode > 0) {
			return BAN_CODE::ANTICHEAT4;
		}
		else {
			return BAN_CODE::ANTICHEAT3;
		}
	}
	if (parsed_currencies.contains("0xBATTLES1")) {
		int statusCode = parsed_currencies["0xBATTLES1"];
		if (statusCode > 0) {
			return BAN_CODE::SUSPECT;
		}
	}
	return BAN_CODE::NOT_BANNED;
}

parsed_balances* balhook(parsed_balances* self, void* param_1, void* param_2) {
	parsed_balances* balances = PLH::FnCast(obalfunc, balhook)(self, param_1, param_2);
	try {
		std::string currencies = std::string(balances->currencies);
		nlohmann::json parsed_currencies = nlohmann::json::parse(currencies);
		
		ban_status = parseForBan(parsed_currencies);
	}
	catch (std::exception ex) {
		ilog(LogLevel::Error, "Ban check error: %s", ex.what());
	}
	return balances;
}

auto balances_hook::apply() -> bool {
	//40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 48 8B ?? 48 83 ?? ?? 4D 8B ?? 4C 8B ?? 48 8B ?? E8 ?? ?? ?? ?? 33
	uintptr_t fp = (uintptr_t)memory::FindSig("40 ?? 53 56 57 41 ?? 41 ?? 41 ?? 48 8B ?? 48 83 ?? ?? 4D 8B ?? 4C 8B ?? 48 8B ?? E8 ?? ?? ?? ?? 33");

	this->create_detour(fp, (uint64_t)&balhook, (uint64_t*)&obalfunc);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto balances_hook::remove() -> bool {
	return this->get_detour()->unHook();
}