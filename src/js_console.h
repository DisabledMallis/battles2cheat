#ifndef SRC_JS_CONSOLE
#define SRC_JS_CONSOLE

#include "render/renderable.h"
#include "player.h"

struct js_console : public renderable {
	js_console();
	void on_render() override;
};

#endif /* SRC_JS_CONSOLE */
