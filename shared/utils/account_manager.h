#ifndef INJECTOR_ACCOUNT_MANAGER
#define INJECTOR_ACCOUNT_MANAGER

#include "dgdata.h"
#include "../json/json.hpp"
#include <Windows.h>

#define ACCOUNTS_PATH (files::getUniformDir()+"/accounts.json")

namespace account_manager {

	struct account_data {
		std::string displayName;
		std::string shortcode;
		std::string nkapiID;
		dgdata::raw_dgdata raw_data;

		account_data(dgdata::raw_dgdata session_data);
		account_data(bool invalid);
		std::string get_displayName();
		std::string get_shortcode();
		bool operator==(const account_data& other);
	};

	static account_data NULL_ACCOUNT = account_data(true);

	void to_json(nlohmann::json& j, const account_data& p);
	void from_json(const nlohmann::json& j, account_data& p);

	void save_file(HANDLE file_handle, dgdata::raw_dgdata content);

	std::vector<account_data> get_all_accounts();
	account_data get_account(std::string shortcode);
	account_data logged_in();
	void save_account(account_data to_save);
	bool remove_account(account_data to_remove);
	void login(account_data to_login);
	void logout();
	std::string get_session_path();
};

#endif