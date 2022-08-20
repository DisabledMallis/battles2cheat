#ifndef INJECTOR_UTILS_STRCONVERT
#define INJECTOR_UTILS_STRCONVERT

#include <string>

namespace strconvert {
	std::string ws_to_s(std::wstring text);
	std::wstring s_to_ws(std::string text);
};

#endif