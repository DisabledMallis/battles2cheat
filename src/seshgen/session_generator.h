#ifndef SRC_SESHGEN_SESSION_MANAGER
#define SRC_SESHGEN_SESSION_MANAGER

#include <string>
#include <functional>
#include "../render/renderable.h"
#include <json/json.hpp>
//#include "../module_storage.h"

struct user_info {
	std::string nkapiID;
	std::string displayName;
	int clan;
	std::string country;
	std::string continent;
	int avatar;
	bool online;
	int onlineApp;
	std::string providersAvailable[1];
	int access;
	int age;
	std::string shortcode;
	std::string safeName;
	std::string email;
	std::string password;
};
struct session_info {
	std::string sessionID;
	size_t expires;
};
//This is everything to change accounts on the fly
struct auth_info {
	user_info user;
	session_info session;
};

namespace session_generator {
	//session_generator() {};
	std::string generate_random_str(size_t length);
	//void send_transactions(request_data* transactions);
	std::string make_request(std::string endpoint, nlohmann::json body);
	std::string create_account(std::string username, std::string email, std::string password);
	std::string login_account(std::string username, std::string email, std::string password);
	auth_info* generate_session();
};
#endif