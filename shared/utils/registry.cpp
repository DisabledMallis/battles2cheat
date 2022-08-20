#include "registry.h"
#include "../logger.h"

void registry::read_value(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text = false) {

    if (value_buffer == nullptr) {
        throw std::exception("value_buffer must be a char[MAX_PATH]!");
        return;
    }

    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_READ, &hKey);
    if (status != ERROR_SUCCESS) {
        ilog(LogLevel::Error, "Failed to open registry key '%s'", path.c_str());
        throw std::exception("Failed to open registry key!");
        return;
    }

    DWORD dwLen = MAX_PATH;
    DWORD dwType = 0;
    status = RegQueryValueExA(hKey, key.c_str(), nullptr, &dwType, (BYTE*)value_buffer, &dwLen);
    if (status != ERROR_SUCCESS) {
        ilog(LogLevel::Error, "Failed to read registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to read registry key!");
        return;
    }
    ilog("dwlen: %x", dwLen);
    if (is_text && dwLen <= MAX_PATH) {
        value_buffer[dwLen] = 0;
    }

    return;
}

std::string registry::read_string(HKEY base_key, std::string path, std::string key) {
    char value_buffer[MAX_PATH];
    read_value(base_key, path, key, value_buffer);
    return std::string(value_buffer);
}

int registry::read_dword(HKEY base_key, std::string path, std::string key) {
    int value_buffer;
    read_value(base_key, path, key, (char*)&value_buffer);
    return value_buffer;
}

void registry::write_value(HKEY base_key, std::string path, std::string key, char* value_buffer, bool is_text)
{
    if (value_buffer == nullptr) {
        throw std::exception("value_buffer must be a char[MAX_PATH]!");
        return;
    }

    HKEY hKey;
    LSTATUS status = RegOpenKeyExA(base_key, path.c_str(), 0, KEY_SET_VALUE, &hKey);
    if (status != ERROR_SUCCESS) {
        status = RegCreateKeyA(base_key, path.c_str(), &hKey);
        if (status != ERROR_SUCCESS) {
            throw std::exception("Failed to create a registry key");
            return;
        }
    }

    DWORD dwLen = is_text ? MAX_PATH : sizeof(int);
    DWORD dwType = is_text ? REG_SZ : REG_DWORD;
    status = RegSetValueExA(hKey, key == "(Default)" ? NULL : key.c_str(), 0, dwType, (BYTE*)value_buffer, dwLen);
    if (status != ERROR_SUCCESS) {
        ilog(LogLevel::Error, "Failed to write registry key '%s' in path '%s'", key.c_str(), path.c_str());
        throw std::exception("Failed to write registry key!");
        return;
    }

    return;
}

void registry::write_string(HKEY base_key, std::string path, std::string key, std::string value)
{
    return write_value(base_key, path, key, (char*)value.c_str(), true);
}

void registry::write_dword(HKEY base_key, std::string path, std::string key, int value)
{
    return write_value(base_key, path, key, (char*)&value, false);
}
