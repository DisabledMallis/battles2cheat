#include "config.h"

#include "logger.h"

nlohmann::json config::readFile()
{
	HANDLE hConfig = files::open_file(CONFIG_PATH);
	std::pair<char*, size_t> fileContent = files::read_file(hConfig);
	files::close_file(hConfig);

	try {
		return nlohmann::json::parse(std::string(fileContent.first, fileContent.second));
	}
	catch (std::exception ex) {
		ilog("Failed to parse config file: %s", ex.what());
		return nlohmann::json::object();
	}
}
void config::saveFile(nlohmann::json json) 
{
	HANDLE hConfig = files::open_file(CONFIG_PATH);
	std::string jsonDump = json.dump();
	files::save_file(hConfig, (char*)jsonDump.c_str(), jsonDump.size());
	files::close_file(hConfig);
}

int config::getMenuKey()
{
	nlohmann::json configFile = readFile();
	if (configFile.find("ui_toggle") != configFile.end()) {
		nlohmann::json optUiToggle = configFile.at("ui_toggle");
		if (optUiToggle.is_number_integer()) {
			return optUiToggle;
		}
	}
	return VK_OEM_4;
}

void config::setMenuKey(int value)
{
	nlohmann::json configFile = readFile();
	configFile["ui_toggle"] = value;
	saveFile(configFile);
}
