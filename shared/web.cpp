#include "web.h"

#include "logger.h"

std::string web::form_args(std::map<std::string, std::string> args)
{
	std::stringstream arg_stream;
	int counter = 0;
	for (auto const& [name, value] : args) {
		if (counter == 0) {
			arg_stream << "?";
		}
		else {
			arg_stream << "&";
		}
		arg_stream << name;
		arg_stream << "=";
		arg_stream << value;
		counter++;
	}
	return arg_stream.str();
}

int web::make_request(httplib::Client& cli, std::string route, std::map<std::string, std::string> args, std::string* response, httplib::Progress progressLambda)
{
	std::string url_args = web::form_args(args);
#ifdef _DEBUG
	ilog("Formed url args: %s", url_args.c_str());
	ilog("Full route: %s", (route + url_args).c_str());
#endif
	httplib::Result res = cli.Get((route + url_args).c_str(), progressLambda);
	if (!res) {
		response = nullptr;
#ifdef _DEBUG
		ilog(LogLevel::Error, "HTTP Request failed!");
#endif
		return 0;
	}
	if (res->status != 200) {
#ifdef _DEBUG
		ilog(LogLevel::Warning, "Request returned with %i", res->status);
		ilog(LogLevel::Info, "Request Body:\n%s", res->body.c_str());
#endif // DEBUG
	}
	*response = std::string(res->body.c_str(), res->body.size());
	return res->status;
}