#ifndef SRC_RENDER_RENDER_MGR
#define SRC_RENDER_RENDER_MGR

#include "renderable.h"

class render_mgr {
public:
	static void registerListener(renderable*);
	static void dispatchRender();
};

#endif /* SRC_RENDER_RENDER_MGR */
