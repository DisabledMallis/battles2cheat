#include "render_mgr.h"
#include <vector>
#include <Windows.h>
#include <config.h>

std::vector<renderable*> listeners;
std::vector<renderable*> js_windows;

int uiToggleKey = VK_OEM_4;
bool toggled = true;
bool show_demo = false;

void render_mgr::registerListener(renderable* listener) {
	listeners.push_back(listener);
	uiToggleKey = config::getMenuKey();
}
void render_mgr::dispatchRender() {
	ImGuiIO& io = ImGui::GetIO();
	if (io.KeysDownDuration[uiToggleKey] == 0.0) {
		toggled = !toggled;
	}
#if CHEAT_ONLY > 0
	for (auto renderable : listeners) {
		if (renderable->get_name() == "cheat menu") {
			renderable->toggled = toggled;
		}
	}
#elif CHEAT_ONLY == 0
	ImGui::Begin("Windows");
	for (auto renderable : listeners) {
		ImGui::Checkbox(renderable->get_name().c_str(), &renderable->toggled);
	}
	ImGui::Checkbox("ImGui Demo", &show_demo);
	if(show_demo) {
		ImGui::ShowDemoWindow();
	}
	ImGui::End();
#endif
	for (auto renderable : listeners) {
#if CHEAT_ONLY == 0
		if (renderable->get_name() == "cheat menu") {
			renderable->on_render();
		}
		else if (renderable->toggled) {
#endif
			renderable->on_render();
#if CHEAT_ONLY == 0
		}
#endif
	}
	for (auto renderable : js_windows) {
		renderable->on_render();
	}
}