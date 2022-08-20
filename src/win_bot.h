#ifndef SRC_WIN_BOT
#define SRC_WIN_BOT

#include "render/renderable.h"

struct win_bot : public renderable {
	win_bot();
	void on_render() override;
};

#endif /* SRC_WIN_BOT */