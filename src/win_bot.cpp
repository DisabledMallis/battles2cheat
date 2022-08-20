#include "win_bot.h"
#include <mutex>
#include <chrono>
#include <Windows.h>


/* B2C Trophy generator / win bot */
//How it works, aka implementation standard
//1. Find "Battle" button from main menu
//2. Call the "Battle" button click function, OR simulate a click
//3. Repeat for Hero & Tower selection
//4. Hang loading thread for arbitrary amount of time
//5. Release the loading thread
//6. If the victory screen isn't loaded within 10 seconds, proceed to step 7 and on. Otherwise jump to step 9
//7. Find the forfeit button from CGameHudScreen
//8. Forfeit the game OR simulate a forfeit click
//9. On victory/defeat screen, find "Ok" button
//10. Wait for CMainMenuScreen to fully load, then repeat step 1

bool bot_active = false;
bool thread_locked = false;
extern std::mutex load_locker;
extern void(*gscr_pre_lock)();
extern void(*gscr_unlock)();
int seconds_till_unlock = 60 * 5; //5 minutes until unlock by default
std::chrono::time_point locked_at = std::chrono::system_clock::now();
void on_lock() {
	std::chrono::time_point time_now = std::chrono::system_clock::now();

	//Not locked, but now is
	locked_at = time_now;
	if (bot_active) {
		thread_locked = true;
		//Sleep until unlock
		Sleep(seconds_till_unlock * 1000);
	}
};
void on_unlock() {
	thread_locked = false;
};

win_bot::win_bot() : renderable::renderable("win bot") {
	gscr_pre_lock = on_lock;
	gscr_unlock = on_unlock;
};

void win_bot::on_render() {
	//Editor window
	ImGui::Begin("Win bot");
	ImGui::TextWrapped("AFK Win bot");
	ImGui::Checkbox("Active", &bot_active);
	ImGui::InputInt("Continue after", &seconds_till_unlock, 1.0f, 10.0f);

	std::chrono::time_point time_now = std::chrono::system_clock::now();

	if (thread_locked) {
		long long remaining = seconds_till_unlock - std::chrono::duration_cast<std::chrono::seconds>(time_now - locked_at).count();
		ImGui::TextWrapped("Thread is paused - about %i seconds remaining", remaining);
	}

	ImGui::End();
};