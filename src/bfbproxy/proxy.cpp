#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>

#include "proxy.h"
#include <profile.h>

#ifdef USE_HTTPS
httplib::SSLServer svr("./cert.pem", "./key.pem");
#else
httplib::Server svr;
#endif

extern profile* user_profile;

void bfb_proxy::send_analytics() {
	//Send analytics
	while (!user_profile) {
		//Wait for user profile data
		Sleep(10);
	}

	//The remaining code in this function has been redacted
}

void bfb_proxy::start_server()
{
	send_analytics();
	//Emulates v1.0.0 anticheat LOL
	svr.Post("/multi", [](const httplib::Request& req, httplib::Response& res) {
		res.set_content("{\"responses\":[{\"status\":200,\"body\":\"{\\\"send\\\":true,\\\"error\\\":\\\"\\\"}\"}]}", "application/json");
	});

	svr.listen("127.0.0.1", 5743);
};

void bfb_proxy::stop_server()
{
	svr.stop();
};

