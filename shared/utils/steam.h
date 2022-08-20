#ifndef INJECTOR_STEAM
#define INJECTOR_STEAM

#include <string>

namespace steam {
	std::string get_steam_dir();
	std::string get_userdata_dir();
	std::string get_user_id();
};

#endif