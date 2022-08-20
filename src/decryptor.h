#ifndef SRC_DECRYPTOR
#define SRC_DECRYPTOR

#include "enc_val.h"
#include "render/renderable.h"

class decryptor : public renderable {
public:
	decryptor();
	void on_render() override;
};

#endif /* SRC_DECRYPTOR */
