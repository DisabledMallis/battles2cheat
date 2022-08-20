#ifndef INJECTOR_REGISTRY
#define INJECTOR_REGISTRY

#include <string>
#include <Windows.h>

namespace registry {
	void read_value(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text);
	std::string read_string(HKEY base_key, std::string path, std::string key);
	int read_dword(HKEY base_key, std::string path, std::string key);

	void write_value(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text);
	void write_string(HKEY base_key, std::string path, std::string key, std::string value);
	void write_dword(HKEY base_key, std::string path, std::string key, int value);
};

#endif