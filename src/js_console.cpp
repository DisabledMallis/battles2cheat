#include "js_console.h"
#include "js_wrapper.h"
#include "imgui/TextEditor.h"
#include "strconvert.h"

js_console::js_console() : renderable::renderable("js console") {
};

std::vector<std::string> js_logs;
TextEditor editor;

void js_console::on_render() {
	//Editor window
	ImGui::Begin("JS Editor");
	editor.Render("JS Code");
	ImGui::End();

	ImGui::Begin("JS Executor");
	if(ImGui::Button("Execute from Editor")) {
		std::wstring w_result = js_wrapper::run_script(editor.GetText());
		std::string s_result = strconvert::ws_to_s(w_result);
		js_logs.push_back(s_result);
	}
	ImGui::SameLine();
	if(ImGui::Button("Reset")) {
		js_wrapper::destroy_runtime();
		js_wrapper::setup_runtime();
	}
	ImGui::SameLine();
	if(ImGui::Button("Clear")) {
		js_logs.clear();
	}
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	for(auto text : js_logs) {
		ImGui::TextWrapped(text.c_str());
	}
	ImGui::EndChild();
	ImGui::End();
};