#ifndef INJECTOR_UTILS_FILES
#define INJECTOR_UTILS_FILES

#include <Windows.h>
#include <string>

namespace files {
	//The current module's path
	std::string getModulePath(std::string module_name);
	//The directory of where a specificed module is located, "(Base)" for getting the .exe's home dir
	std::string getModuleDir(std::string module_name);
	//The current execution directory
	std::string getExecDir();
	//The current execution file
	std::string getExecPath();
	//Directory to store data universally (no matter where injector is moved, this data is in the same spot)
	std::string getUniformDir();

	HANDLE open_file(std::string file_path);
	std::pair<char*, size_t> read_file(HANDLE file_handle);
	void save_file(HANDLE file_handle, char* content, size_t length);
	void close_file(HANDLE file_handle);
	bool delete_file(std::string file_path);
};

#endif // !INJECTOR_UTILS_FILES
