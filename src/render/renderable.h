#ifndef SRC_RENDER_RENDERABLE
#define SRC_RENDER_RENDERABLE

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui.h>
#include <imgui/imstb_rectpack.h>
#include <imgui/imstb_textedit.h>
#include <imgui/imstb_truetype.h>

#include <string>

class renderable {
	std::string name;
public:
	bool toggled = false;
	renderable(std::string name) {
		this->name = name;
	}
	virtual std::string get_name() {
		return this->name;
	}
	virtual void on_render() {};
};

#endif /* SRC_RENDER_RENDERABLE */
