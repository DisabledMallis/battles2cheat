#include "redshift_display.h"

#include <string>

redshift_display::redshift_display() : renderable::renderable("redshift display") {
};

extern redshift* p_redshift;
void redshift_display::on_render() {
	//Do not leak!!!!
	ImGui::Begin("Redshift Info (AKA Ban bypass toolkit :O)");
	if (p_redshift) {
		ImGui::Text("Url: %s", p_redshift->url.c_str());
		ImGui::Text("Request Url: %s", p_redshift->request_url.c_str());
	}
	ImGui::End();
};