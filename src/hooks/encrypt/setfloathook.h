#ifndef SRC_HOOKS_ENCRYPT_SETFLOATHOOK
#define SRC_HOOKS_ENCRYPT_SETFLOATHOOK

#include "../hook.h"

class setFloatHook : public hook {
public:
	setFloatHook() : hook("enc_set_float") {}
	auto apply() -> bool override;
	auto remove() -> bool override;
};

#endif // !SRC_HOOKS_ENCRYPT_SETFLOATHOOK
