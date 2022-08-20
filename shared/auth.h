#ifndef INJECTOR_UTILS_AUTH
#define INJECTOR_UTILS_AUTH

#include "logger.h"
#include "json/json.hpp"
#include <string>

namespace auth {

	struct authPayload {
		char session_id[MAX_PATH];
		char session_expires[MAX_PATH];
	};

	struct authInfo {
		std::string session_id;
		std::string expiration;
		std::string username;
		int uid;

		authInfo(authPayload payload);
		authInfo(std::string session_id, std::string expiration);

		void downloadUserInfo();
		bool isValid();
	};

	authInfo* readAuthData();
	authInfo* getAuthData();
	void saveAuthData(std::string session_id, std::string expiration);
	void clearAuthData();
	bool setupAuthPipe();
	authInfo* awaitAuthData();
};

#define AUTH_PATH (files::getUniformDir() + "/auth.benc")
#define AUTH_URL "https://forum.b2c.sbs"
#define AUTH_ROUTE "/api/v3/plugins/b2cauth/userInfo"
#define AUTH_PIPE "\\\\.\\pipe\\bpa\\auth"
#define AUTH_PIPE_SIZE sizeof(auth::authPayload)

#endif // INJECTOR_UTILS_AUTH
