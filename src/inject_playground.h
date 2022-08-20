#ifndef SRC_INJECT_PLAYGROUND
#define SRC_INJECT_PLAYGROUND

#include "enc_val.h"
#include "render/renderable.h"

class inject_playground : public renderable {
public:
	inject_playground();
	void on_render() override;
};

#endif /* SRC_INJECT_PLAYGROUND */