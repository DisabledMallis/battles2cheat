#if CHEAT_ONLY == 0
#include "player_info.h"
#include "memory.h"
#include "hooks/hook_manager.h"
#include <string>
#include "game_instance.h"
#include "game_hud.h"

typedef player_data*(__fastcall get_player_data_fun)(void*);
extern get_player_data_fun* get_player_data;
extern CGameHudScreen* p_gamehud;

//extern game_instance* g_instance;

//extern std::vector<player*> players;

player_info::player_info() : hook::hook("update game"), renderable::renderable("player_info") {
}

bool showData = false;
void player_info::on_render() {
	ImGui::Begin("Player info");
	/*
	if(g_instance) {
		ImGui::Text("Game instance: PTR[%p], UUID[%x]", g_instance, g_instance->client_uuid);
	}
	*/
	//ImGui::Checkbox("Show data", &showData);
	player* players = p_gamehud->pGameScreen->pGameRunner->players;
	for(int i = 0; i < 2; i++) {
		player& player = players[i];
		no_clue_b* no_clue_ptr_b = player.weirdo;
		no_clue_a* no_clue_ptr_a = no_clue_ptr_b->no_clue_ptr_a;
		if(!no_clue_ptr_a) {
			ImGui::Text("Player is invalid");
			break;
		}
		size_t gets_player_ptr = (size_t)no_clue_ptr_a->gets_player;
		if(!no_clue_ptr_a) {
			ImGui::Text("Player is invalid");
			break;
		}
		gets_player_ptr += 0x50;
		ImGui::PushID(i);
		char ptr_buffer_1[MAX_PATH];
		sprintf(ptr_buffer_1, "%p", gets_player_ptr);
		ImGui::InputText("Gets player ptr", ptr_buffer_1, MAX_PATH);
		player_data* data = get_player_data((void*)gets_player_ptr);
		char ptr_buffer_2[MAX_PATH];
		sprintf(ptr_buffer_2, "%p", data);
		ImGui::InputText("Data ptr", ptr_buffer_2, MAX_PATH);
		ImGui::PopID();

		if(data) {
			if(data->always_2 != 2) {
				//OOPSIE WHOOPSIE
				//TIme to sdeelete

				ImGui::Text("Player is invalid");
				break;
			}
			
			ImGui::Text("Index: %x", i);
			ImGui::Text("Username: %s", player.username.c_str());
			ImGui::Text("Masked name: %s", player.masked_name.c_str());
			
			ImGui::Text("Cash: %f", data->cash.decrypt_value());
			ImGui::Text("Eco: %f", data->eco.decrypt_value());
			ImGui::Text("Health: %f", data->health.decrypt_value());
			ImGui::Text("Next pay: %f", data->next_pay.decrypt_value());

			ImGui::Text("Tower count: %x", player.tower_count);
			//std::vector<void*>* towers = player->tower_vec;
			//for(auto tower : *towers) {
			//	ImGui::PushID((int)tower);
			//	char buf[32];
			//	sprintf(buf, "%p", tower);
			//	ImGui::InputText("", buf, 32);
			//	ImGui::PopID();
			//}
		}
	}
	ImGui::End();
}

auto player_info::apply() -> bool {
	return true;
}

auto player_info::remove() -> bool {
	return true;
}
#endif