#ifndef SRC_SCENE
#define SRC_SCENE

#include <nuv.h>

namespace Scene {
	class Entity {
		char pad_0000[32]; //0x0000
		nuv::string entity_tag; //0x0020
		char pad_0040[32]; //0x0040
	};
	class Empty : public Entity {};
};

#endif // !SRC_SCENE
