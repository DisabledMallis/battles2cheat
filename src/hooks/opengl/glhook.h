#ifndef SRC_HOOKS_OPENGL_GLHOOK
#define SRC_HOOKS_OPENGL_GLHOOK

#include "../hook.h"

class glhook : public hook {
public:
	glhook() : hook("glfwSwapBuffers")
	{}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif /* SRC_HOOKS_OPENGL_GLHOOK */
