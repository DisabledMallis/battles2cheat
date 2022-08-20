#include "cheat_menu.h"
#include "game_instance.h"
#include "profile.h"
#include "hooks/hook_manager.h"
#include <shellapi.h>
#include "redshift.h"
#include <mutex>
#include "game_hud.h"
#include <BanCodes.h>
#include <logger.h>
#include <auth.h>
#include <config.h>
#include <Simulation.h>
#include "seshgen/session_generator.h"

typedef Simulation::Components::SPlayerComponent*(__fastcall get_player_data_fun)(void*);
get_player_data_fun* get_player_data;

extern profile* user_profile;
extern CGameHudScreen* p_gamehud;
extern BAN_CODE ban_status;

cheat_menu::cheat_menu() : renderable::renderable("cheat menu"), hook::hook("cheat menu") {
	get_player_data = (get_player_data_fun*)memory::FindSig("48 89 ?? ?? ?? 48 89 ?? ?? ?? 48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? B2 ?? E8 ?? ?? ?? ?? 48 8B ?? 33 F6 48 85 ?? 74 ?? 0F 1F ?? ?? ?? ?? ?? ?? 48 8B ?? 48 8B ?? FF 50 ?? 3B F0 73 ?? 48 8B ?? 8B D6 48 8B ?? FF 50 ?? 84 C0 74 ?? 48 8B ?? 8B D6 48 8B ?? FF 50 ?? 90 48 8D ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 0F B6 ?? B8 ?? ?? ?? ?? D2 E0 A8 ?? 75 ?? FF C6 EB ?? 48 85 ?? 74 ?? 48 8B ?? 48 8B ?? FF 50 ?? 8B F8 EB ?? 33 FF 48 85 ?? 74 ?? 90 48 8B ?? 48 8B ?? FF 50 ?? 3B F8 73 ?? 48 8B ?? 8B D7 48 8B ?? FF 50 ?? 84 C0 74 ?? 48 8B ?? 8B D7 48 8B ?? FF 50 ?? 90 48 8D ?? ?? 48 8B ?? E8 ?? ?? ?? ?? 0F B6 ?? B8 ?? ?? ?? ?? D2 E0 A8 ?? 75 ?? FF C7 EB ?? 3B F7 73 ?? 48 8B ?? 8B D6 48 8B ?? FF 50 ?? 90 EB ?? 33 C0 48 8B ?? ?? ?? 48 8B ?? ?? ?? 48 8B ?? ?? ?? 48 83 C4 ?? 5F C3 CC 48 3B");
};

//A function to "safely" load the data from a player
Simulation::Components::SPlayerComponent* get_data_safe(player& player) {
	if(false) {
invalid_player:
		ImGui::Text("Player is invalid");
		return nullptr;
	}
	no_clue_b* no_clue_ptr_b = player.weirdo;
	if(!no_clue_ptr_b) {
		goto invalid_player;
	}
	no_clue_a* no_clue_ptr_a = no_clue_ptr_b->p_no_clue_a;
	if(!no_clue_ptr_a) {
		goto invalid_player;
	}
	size_t gets_player_ptr = (size_t)no_clue_ptr_a->getsPlayer;
	if(!no_clue_ptr_a) {
		goto invalid_player;
	}
	gets_player_ptr += 0x50;
	//ImGui::Text("Gets player ptr %p", gets_player_ptr);
	return get_player_data((void*)gets_player_ptr);
}

std::map<std::string, float*> input_buffers;

std::map<std::string, bool> input_loop;
std::map<std::string, int> input_type; // 0 = No Input, 1 = Recursive Add, 2 = Recursive Set

void value_editor(std::string val_name, enc_val_float* the_val, const char* displayName, size_t playerUUID) {
	if(!input_buffers.count(val_name)) {
		input_buffers[val_name] = new float(the_val->decrypt_value());
	}

	if(!input_loop.count(val_name)) {
		input_loop[val_name] = false;
	}
	if(!input_type.count(val_name)) {
		input_type[val_name] = 0;
	}

	ImGui::Text("%s: %f", displayName, the_val->decrypt_value());
	ImGui::InputFloat(displayName, input_buffers[val_name], 1.0F, 100.0F);
	ImGui::PushID((int)the_val->decrypt_value() + playerUUID);
	if(ImGui::Button("Set")) {
		*the_val = *input_buffers[val_name];
		if (input_loop[val_name])
			input_type[val_name] = 1;
	}

	ImGui::SameLine();
	ImGui::Checkbox("Loop", &input_loop[val_name]);
	ImGui::PopID();
}

void string_field(nuv::string* fieldPtr, const char* text) {
	char* p_fieldTxt = (char*)fieldPtr->c_str();
	size_t len_u = fieldPtr->length()+0xF;
	ImGui::InputText(text, p_fieldTxt, len_u);
	*fieldPtr = p_fieldTxt;
}

const char* bloon_types[] {
	"Red",			//0
	"RedRush",		//1
	"Blue",			//2
	"BlueRush",		//3
	"Green",		//4
	"GreenRush",	//5
	"Yellow",		//6
	"YellowRush",	//7
	"Pink",			//8
	"PinkRush",		//9
	"White",		//10
	"WhiteRush",	//11
	"Black",		//12
	"BlackRush",	//13
	"Purple",		//14
	"PurpleRush",	//15
	"Zebra",		//16
	"ZebraRush",	//17
	"Lead",			//18
	"LeadRush",		//19
	"Rainbow",		//20
	"RainbowRush",	//21
	"Ceramic",		//22
	"CeramicRush",	//23
	"MOAB",			//24
	"MOABRush",		//25
	"BFB",			//26
	"BFBRush",		//27
	"ZOMG",			//28
	"ZOMGRush",		//29
	"DDT",			//30
	"DDTRush",		//31
	"BAD",			//32
	"BADRush"		//33
};

float animX = 0;

/* Cheat values */
std::string username = "";
player* players = nullptr;
player* localPlayer = nullptr;
Simulation::Components::SPlayerComponent* localPlayerData = nullptr;
void cleanPointers() {
	username = "";
	players = nullptr;
	localPlayer = nullptr;
	localPlayerData = nullptr;
}

/*Cheat toggles*/
//Bypasses
bool bp0 = false;
bool bp1 = false; //Anticheat bypass 1
bool bp2 = true; //Steam bypass
bool bp3 = true; //Anticheat bypass 2
bool bp2fail = false;
extern size_t blockedRequests;

//Player
bool doubleEco = false;
float ecoSkip = 0;
bool reduceDamage = false;
float reducePercent = 0;

//Util
bool unlockAllTowers = false;
bool unlockAllUpgrades = false;

//Exploit
bool cancelSends = false;

//Blatant
bool infiniteCash = false;
bool wasCashInf = false;
float cashBeforeInf = 0.0f;
#define CASH_INF 500000.0f;
bool addCashNow = false;
float cashAdd = 0.0f;
#define CASH_MAX 100000.0f
bool godmode = false;
bool gmValLock = true;
bool gmCancel = false;
bool infTowerBoosts = false;
bool infBloonBoosts = false;

//Settings
extern int uiToggleKey;
bool changingKey = false;
bool safeMode = true;

auth_info* lastGen;

void cheat_menu::on_render() {

	ImGuiIO& io = ImGui::GetIO();

	float winWidth = io.DisplaySize.x/3;
	float delta = io.DeltaTime;// / 1000;

	if (toggled) {
		animX += delta;
		if (animX >= 1) {
			animX = 1;
		}
	}
	else {
		animX -= delta;
		if (animX <= 0) {
			animX = 0;
		}
	}

	float winX = (ImGui::EaseOutExpo(animX) * winWidth) - winWidth;

	//ilog("delta: %f animX: %f winX: %f", delta, animX, winX);
	ImGui::SetNextWindowPos(ImVec2(winX, 0));
	ImGui::SetNextWindowSize(ImVec2(winWidth, io.DisplaySize.y));
	ImGui::Begin("B2C", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::PushTextWrapPos(winWidth - 25); //Make all text wrap to the current menu size
	ImGui::PushFont(io.Fonts->Fonts[0]); // Arial font size 16

	ImGui::PushFont(io.Fonts->Fonts[1]); // Arial font size 72
	ImGui::Text("B2C");
	ImGui::PopFont(); // Pop Arial sz72

	auth::authInfo* userInfo = auth::getAuthData();
	if (!userInfo || !userInfo->isValid()) {
		ImGui::Text("Loading B2C version %s...", B2C_VERSION);
		ImGui::PopFont(); // Pop Arial sz16
		ImGui::PopTextWrapPos(); //Pop the text wrapping
		ImGui::End();
		return;
	}
	else {
		ImGui::Text("Welcome %s", userInfo->username.c_str());

		ImGui::Text("B2C version %s", B2C_VERSION);
		ImGui::Rainbow();
	}

	ImGui::Text("Ban status: ");
	ImGui::SameLine();
	switch (ban_status) {
	case BAN_CODE::NOT_BANNED:
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Not banned");
		break;
	case BAN_CODE::ANTICHEAT:
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Banned (Anticheat type 1)");
		break;
	case BAN_CODE::ANTICHEAT2:
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Banned (Anticheat type 2)");
		break;
	case BAN_CODE::ANTICHEAT3:
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Banned (Silent/Shadow)");
		break;
	case BAN_CODE::ANTICHEAT4:
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Banned (Unknown reason)");
		break;
	case BAN_CODE::SUSPECT:
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "You have been flagged by an opponent");
		break;
	case BAN_CODE::STEAM:
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Banned (Steam)");
		break;
	default:
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Unknown status (Hackerpool?)");
	}

	/*Settings*/
	ImGui::TextWrapped("Menu toggle key:");
	ImGui::SameLine();
	const char* buttonTxtFinal = "NONE";
	char buttonTxt[MAX_PATH];
	GetKeyNameTextA(MapVirtualKeyW(uiToggleKey, MAPVK_VK_TO_VSC) << 16, buttonTxt, sizeof(buttonTxt));
	if (changingKey) {
		buttonTxtFinal = "...";
		for (int i = VK_BACK; i < VK_OEM_CLEAR; i++) {
			if (io.KeysDownDuration[i] == 0.0f) {
				config::setMenuKey(i);
				uiToggleKey = config::getMenuKey();
				changingKey = false;
			}
		}
	}
	else {
		buttonTxtFinal = buttonTxt;
	}
	if (ImGui::Button(buttonTxtFinal)) {
		changingKey = true;
	}

	ImGui::Checkbox("Safe Mode", &safeMode);
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text("Prevents you from setting unsafe values");
		ImGui::TextColored(ImColor(255, 0, 0), "NOTE: You can still be banned! This is just an option to *help* prevent it.");
		ImGui::EndTooltip();
	}

#ifdef _DEBUG
	if (ImGui::Button("New Account")) {
		lastGen = session_generator::generate_session();
	}
	if (lastGen) {
		ImGui::InputText("Email", (char*)lastGen->user.email.c_str(), lastGen->user.email.size());
		ImGui::InputText("Password", (char*)lastGen->user.password.c_str(), lastGen->user.password.size());
	}
#endif

	/* Cheats */
	if (ImGui::CollapsingHeader("Bypasses")) {
		//Its really just the date - author. Tryna make it look cryptic to confuse noobies :)
		if(bp2fail) {
			bp2 = false;
			ImGui::TextColored(ImColor(1, 0, 0), "Steam bypass is incompatible");
			ImGui::BeginDisabled();
		}
		ImGui::Checkbox("Steam Ban Bypass", &bp2);
		if (bp2fail) {
			ImGui::EndDisabled();
		}
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Fully bypasses Steam ID bans");
			ImGui::EndTooltip();
		}
		
		ImGui::Checkbox("Anticheat Bypass 2 (Patched)", &bp3);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Prevents the game from sending packets that could lead to a ban");
			ImGui::EndTooltip();
		}

		if (safeMode) {
			ImGui::BeginDisabled();
		}
		ImGui::Checkbox("Anticheat Bypass 1 (Patched)", &bp1);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Prevents the game from sending packets that could lead to a ban");
			ImGui::EndTooltip();
		}
		if (safeMode) {
			ImGui::EndDisabled();
		}
	}
	else {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Tools to help avoid bans");
			ImGui::EndTooltip();
		}
	}

	//Retrieve datas
	if (user_profile) {
		username = user_profile->user.displayName.getString();
	}
	else {
		ilog("No user profile found");
	}
	if (p_gamehud) {
		auto pGameScreen = p_gamehud->pGameScreen;
		if (pGameScreen) {
			auto pGameRunner = pGameScreen->pGameRunner;
			if (pGameRunner) {
				players = pGameRunner->players;
				for (int i = 0; i < 2; i++) {
					player& iPlayer = players[i];
					if (iPlayer.username == username) {
						localPlayer = &iPlayer;
						localPlayerData = get_data_safe(iPlayer);
					}
				}
			}
			else {
#ifdef _DEBUG
				ilog("No gamerunner found");
#endif
				cleanPointers();
			}
		}
		else {
#ifdef _DEBUG
			ilog("No gamescreen found");
#endif
			cleanPointers();
		}
	}
	else {
#ifdef _DEBUG
		ilog("No gamehud found");
#endif
		cleanPointers();
	}

	//Cheat menu
	if (ImGui::CollapsingHeader("Player")) {
		/* Eco mod */
		ImGui::Checkbox("Eco Mod", &doubleEco);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Skips part of the Eco timer by a specified percent");
			ImGui::Text("0%% - No time is skipped (6 second Eco timer)");
			ImGui::Text("50%% - Half of the timer is skipped (3 second Eco timer)");
			ImGui::Text("100%% - The timer count down is entirely skipped (0 second Eco timer, use Infinite Cash instead!)");
			ImGui::EndTooltip();
		}
		ImGui::Indent();
		ImGui::SliderFloat("Skip", &ecoSkip, 0, safeMode ? 25 : 100, "%.3f%%", 1);
		ImGui::Unindent();

		/* Damage reducer */
		ImGui::Checkbox("Reduce Damage", &reduceDamage);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Reduces damage taken by a specified percent");
			ImGui::Text("0%% - No damage reduced (1 Blue = 2 HP lost)");
			ImGui::Text("50%% - Half damage is reduced (1 Blue = 1 HP lost)");
			ImGui::Text("100%% - All damage reduced (No HP is ever lost, use Godmode instead!)");
			ImGui::EndTooltip();
		}
		ImGui::Indent();
		ImGui::SliderFloat("Reduce By", &reducePercent, 0, safeMode ? 50 : 100, "%.3f%%", 1);
		ImGui::Unindent();
	}
	else {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Cheats that affect the player");
			ImGui::EndTooltip();
		}
	}

	if (ImGui::CollapsingHeader("Utils")) {
		ImGui::Checkbox("Unlock all towers", &unlockAllTowers);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Allows you to use all towers");
			ImGui::TextColored(ImColor(255, 0, 0), "Enable before queueing");
			ImGui::EndTooltip();
		}
		ImGui::Checkbox("Unlock all upgrades", &unlockAllUpgrades);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Allows you to use all upgrades");
			ImGui::EndTooltip();
		}
	}
	else {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Cheats that make the game less annoying");
			ImGui::EndTooltip();
		}
	}

	if (ImGui::CollapsingHeader("Exploit")) {
		ImGui::Checkbox("Cancel opponent sends", &cancelSends);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Prevents your opponent from sending you bloons");
			ImGui::EndTooltip();
		}
	}

	if (safeMode) {
		ImGui::BeginDisabled();
	}
	if (ImGui::CollapsingHeader("Blatant")) {
		/* Add cash */
		ImGui::InputFloat("##Cash difference", &cashAdd, 1.0f, 10.0f, "%.2f", 0);
		ImGui::SameLine();
		if (ImGui::Button("Add cash")) {
			addCashNow = true;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Give yourself the specified amount of cash");
			ImGui::EndTooltip();
		}

		/* Infinite cash */
		ImGui::Checkbox("Infinite Cash", &infiniteCash);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Dangerous features");
			ImGui::TextColored(ImColor(255, 0, 0), "Locks your cash at 500k");
			ImGui::EndTooltip();
		}

		/* Godmode */
		ImGui::Checkbox("Godmode", &godmode);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Prevents bloons from doing damage");
			ImGui::EndTooltip();
		}
		ImGui::Indent();
		ImGui::Checkbox("Lock value", &gmValLock);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Locks the value client side");
			ImGui::TextColored(ImColor(255, 0, 0), "If disabled, you can still lose; your client will still send a death packet");
			ImGui::EndTooltip();
		}
		ImGui::Checkbox("Cancel packet", &gmCancel);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Stops the game from sending health related packets");
			ImGui::EndTooltip();
		}
		ImGui::Unindent();

		/* Tower & Bloon boosts */
		ImGui::Checkbox("Unlimited Tower boosts", &infTowerBoosts);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Locks the tower boost count at 3");
			ImGui::EndTooltip();
		}
		ImGui::Checkbox("Unlimited Bloon boosts", &infBloonBoosts);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Locks the bloon boost count at 3");
			ImGui::EndTooltip();
		}
	}
	else {
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text("Dangerous features");
			ImGui::TextColored(ImColor(255, 0, 0), "These all have a HIGH RISK OF BAN");
			ImGui::EndTooltip();
		}
	}
	if (safeMode) {
		ImGui::EndDisabled();
	}

	ImGui::PopFont(); // Pop Arial sz16
	ImGui::PopTextWrapPos(); //Pop the text wrapping

	ImGui::End();

	//Apply cheats (No more imgui calls allowed)
	if (localPlayerData) {
		if (doubleEco) {
			if (ecoSkip > 0) {
				float skipAt = 6 * (ecoSkip / 100);
				auto& next_pay = localPlayerData->next_pay;
				if (next_pay < skipAt) {
					next_pay = skipAt;
				}
			}
		}
		if (addCashNow) {
			float resultAmount = localPlayerData->cash + cashAdd;
			if (resultAmount >= CASH_MAX) {
				resultAmount = CASH_MAX;
			}
			localPlayerData->cash = resultAmount;
			addCashNow = false;
		}
		if (infiniteCash) {
			if (!wasCashInf) {
				cashBeforeInf = localPlayerData->cash.decrypt_value();
				wasCashInf = true;
			}
			localPlayerData->cash = CASH_INF;
		}
		else {
			if (wasCashInf) {
				localPlayerData->cash = cashBeforeInf;
				wasCashInf = false;
			}
		}
		if (infTowerBoosts) {
			auto* towerBoostComponent = localPlayerData->GetTowerBoostComponent();
			//ilog("Tower boost component: %p", towerBoostComponent);
			if (towerBoostComponent) {
				towerBoostComponent->count = 3;
			}
		}
		if (infBloonBoosts) {
			auto* bloonBoostComponent = localPlayerData->GetBloonBoostComponent();
			//ilog("Bloon boost component: %p", bloonBoostComponent);
			if (bloonBoostComponent) {
				bloonBoostComponent->count = 3;
			}
		}
	}
};
auto cheat_menu::apply() -> bool {
	/*
	this->create_detour((uintptr_t)updateGame, (uint64_t)&updateHook, (uint64_t*)&oUpdateGame);
	if(this->get_detour()->hook()) {
		return true;
	}
	*/
	return false;
};
auto cheat_menu::remove() -> bool {
	return true;// this->get_detour()->unHook();
};