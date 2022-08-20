#include "steam.h"
#include "registry.h"
#include "../logger.h"

std::string steam::get_steam_dir()
{
	return registry::read_string(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", "InstallPath");
}

std::string steam::get_userdata_dir()
{
	return get_steam_dir() + "\\userdata";
}

std::string steam::get_user_id()
{
	int user_id = registry::read_dword(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam\\ActiveProcess", "ActiveUser");
	std::string s_user_id = std::to_string(user_id);
	return s_user_id;
}
