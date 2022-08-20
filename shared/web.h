#ifndef INJECTOR_UTILS_WEB
#define INJECTOR_UTILS_WEB

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib/httplib.h"

#include <map>
#include <string>

namespace web {
	std::string form_args(std::map<std::string, std::string> args);
	int make_request(httplib::Client& cli, std::string route, std::map<std::string, std::string> args, std::string* response, httplib::Progress progressLambda = [](uint64_t progress, uint64_t total) { return true; });
};

#endif // !INJECTOR_UTILS_WEB
