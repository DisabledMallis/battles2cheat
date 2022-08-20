#ifndef SRC_JS_WRAPPER
#define SRC_JS_WRAPPER
#include <string>

namespace js_wrapper {
	void setup_runtime();
	void destroy_runtime();
	std::wstring run_file(std::string filename);
	std::wstring run_script(std::string script);
	std::wstring run_script(std::wstring script);
};
#endif