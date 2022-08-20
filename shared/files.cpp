#include "files.h"
#include "logger.h"
#include <filesystem>
#include <ShlObj_core.h>

std::string files::getModulePath(std::string module_name)
{
	HMODULE module_handle = GetModuleHandleA(module_name == "(Base)" ? NULL : module_name.c_str());
	char path_buffer[MAX_PATH];
	GetModuleFileNameA(module_handle, path_buffer, MAX_PATH);
	return path_buffer;
}

std::string files::getModuleDir(std::string module_name)
{
	std::filesystem::path modulePath = getModulePath(module_name);
	std::filesystem::path parentDir = modulePath.parent_path();
	return parentDir.string();
}

std::string files::getExecDir()
{
	return getModuleDir("(Base)");
}

std::string files::getExecPath()
{
	return getModulePath("(Base)");
}

std::string files::getUniformDir()
{
	char buffer[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, buffer);
	std::string appdataDir = buffer;
	appdataDir += "\\B2C";

	if (CreateDirectoryA(appdataDir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		//ilog("Uniform directory created/already exists");
	}
	else
	{
		//ilog(LogLevel::Error, "Failed to create uniform directory, error: %x", GetLastError());
		return "";
	}

	return appdataDir;
}

HANDLE files::open_file(std::string file_path)
{
	HANDLE hAccFile = CreateFileA(file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hAccFile == INVALID_HANDLE_VALUE) {
		int last_error = GetLastError();
		//ilog(LogLevel::Error, "Failed to open %s file. Error: %x", file_path.c_str(), last_error);
		return NULL;
	}

	return hAccFile;
}

std::pair<char*, size_t> files::read_file(HANDLE file_handle)
{
	int file_size = GetFileSize(file_handle, NULL);
	char* buffer = (char*)malloc(file_size);
	if (buffer == nullptr) {
		ilog(LogLevel::Error, "Couldn't allocate memory for reading %x file", file_handle);
		throw std::exception("Failed to allocate adequate buffer");
	}
	if (!ReadFile(file_handle, buffer, file_size, NULL, NULL)) {
		int last_error = GetLastError();
		ilog(LogLevel::Error, "Failed to read %x file. Error: %x", file_handle, last_error);
		throw std::exception("Failed to read file");
	}

	return std::pair<char*, size_t>(buffer, file_size);
}

void files::save_file(HANDLE file_handle, char* content, size_t length) {
	SetFilePointer(file_handle, 0, NULL, FILE_BEGIN);
	WriteFile(file_handle, content, length, NULL, NULL);
	SetEndOfFile(file_handle);
}

void files::close_file(HANDLE file_handle)
{
	CloseHandle(file_handle);
}

bool files::delete_file(std::string file_path) {
	return DeleteFileA(file_path.c_str());
}
