#pragma once

#include "json/json.hpp"
#include "files.h"

#define CONFIG_PATH (files::getUniformDir()+"/config.json")

namespace config {
	nlohmann::json readFile();
	void saveFile(nlohmann::json json);

	int getMenuKey();
	void setMenuKey(int value);
};