#include "account_manager.h"
#include "../logger.h"
#include "steam.h"
#include "../files.h"

account_manager::account_data::account_data(dgdata::raw_dgdata session_data) {
	//Store the session info
	this->raw_data = session_data;
	nlohmann::json parsed_data = this->raw_data.decode();
	this->displayName = parsed_data["user"]["displayName"];
	this->nkapiID = parsed_data["user"]["nkapiID"];
	this->shortcode = parsed_data["user"]["shortcode"];
}

account_manager::account_data::account_data(bool invalid)
{
	this->displayName = "";
	this->shortcode = "";
	this->nkapiID = "";
}

std::string account_manager::account_data::get_displayName()
{
	return this->displayName;
}

std::string account_manager::account_data::get_shortcode()
{
	return this->shortcode;
}

bool account_manager::account_data::operator==(const account_data& other)
{
	return this->nkapiID == other.nkapiID;
}

void account_manager::to_json(nlohmann::json& j, const account_data& p) {
	j = nlohmann::json{ {"displayName", p.displayName}, {"shortcode", p.shortcode}, {"nkapiID", p.nkapiID}, {"raw_data", p.raw_data} };
}
void account_manager::from_json(const nlohmann::json& j, account_data& p) {
	j.at("displayName").get_to(p.displayName);
	j.at("shortcode").get_to(p.shortcode);
	j.at("nkapiID").get_to(p.nkapiID);
	p.raw_data = j["raw_data"];
}

void account_manager::save_file(HANDLE file_handle, dgdata::raw_dgdata content)
{
	std::vector<char> data = content.data;
	char* buffer = (char*)malloc(data.size() + 1);
	if (buffer == nullptr) {
		ilog(LogLevel::Error, "Couldn't allocate buffer for writing with account_manager");
		return;
	}
	for (size_t i = 0; i < data.size(); i++) {
		buffer[i] = data.at(i);
	}
	buffer[data.size()] = 0;
	if (!WriteFile(file_handle, buffer, data.size(), NULL, NULL)) {
		ilog(LogLevel::Warning, "account_manager Failed to write to file");
		ilog(LogLevel::Error, "Error: %x", GetLastError());
	}
	free(buffer);
}

std::vector<account_manager::account_data> account_manager::get_all_accounts()
{
	HANDLE hAccFile = files::open_file(ACCOUNTS_PATH);
	if (hAccFile == NULL) {
		ilog(LogLevel::Error, "hAccFile was NULL");
		return std::vector<account_data>();
	}

	std::pair<char*, size_t> result = files::read_file(hAccFile);
	std::string sResult = "";
	if (result.second == 0) {
		sResult == "[]";
	}
	else {
		sResult = std::string(result.first, result.second);
	}

	std::vector<account_data> accounts;
	nlohmann::json parsed_file;

	try {
		parsed_file = nlohmann::json::parse(sResult);
	}
	catch (std::exception ex) {
		ilog(LogLevel::Error, "Failed to parse acc file... if this issue persists, it may be ideal to delete it!");
		ilog(LogLevel::Error, ex.what());
		files::close_file(hAccFile);
		//DeleteFileA(ACCOUNTS_PATH.c_str());
		return std::vector<account_data>();
	}

	for (nlohmann::json acc_data : parsed_file) {
		dgdata::raw_dgdata raw_data;
		dgdata::from_json(acc_data["raw_data"], raw_data);
		account_data account(raw_data);
		accounts.push_back(account);
	}

	files::close_file(hAccFile);

	return accounts;
}

account_manager::account_data account_manager::get_account(std::string shortcode)
{
	std::vector<account_data> all_accounts = get_all_accounts();
	for (account_data account : all_accounts) {
		if (account.get_shortcode() == shortcode) {
			return account;
		}
	}
	throw std::exception("Failed to find any account with the shortcode provided!");
}

account_manager::account_data account_manager::logged_in()
{
	std::string session_file = get_session_path();
	if (session_file == "") {
		//ilog("Couldnt find logged in session file");
		return account_manager::NULL_ACCOUNT;
	}
	HANDLE session_handle = CreateFileA(session_file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (session_handle == INVALID_HANDLE_VALUE) {
		return account_manager::NULL_ACCOUNT;
	}
	std::pair<char*, size_t> content = files::read_file(session_handle);
	dgdata::raw_dgdata raw_data(content.first, content.second);
	account_manager::account_data current_user(raw_data);
	files::close_file(session_handle);
	return current_user;
}

void account_manager::save_account(account_data to_save)
{
	std::vector<account_data> all_accounts = get_all_accounts();
	all_accounts.push_back(to_save);

	nlohmann::json json_data = all_accounts;
	std::string str_data = json_data.dump();

	HANDLE acc_file = files::open_file(ACCOUNTS_PATH);
	files::save_file(acc_file, (char*)str_data.c_str(), str_data.size());
	files::close_file(acc_file);
}

bool account_manager::remove_account(account_data to_remove)
{
	bool removed_acc = false;
	std::vector<account_data> all_accounts = get_all_accounts();
	for (size_t i = 0; i < all_accounts.size(); i++) {
		account_data current_account = all_accounts[i];
		if (current_account.displayName == to_remove.displayName) {
			all_accounts.erase(all_accounts.begin() + i);
			removed_acc = true;
			break;
		}
	}

	nlohmann::json json_data = all_accounts;
	std::string str_data = json_data.dump();

	HANDLE acc_file = files::open_file(ACCOUNTS_PATH);
	files::save_file(acc_file, (char*)str_data.c_str(), str_data.size());
	files::close_file(acc_file);
	return removed_acc;
}

void account_manager::login(account_data to_login)
{
	std::string session_path = get_session_path();
	HANDLE session_handle = CreateFileA(session_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	save_file(session_handle, to_login.raw_data);
	files::close_file(session_handle);
}

void account_manager::logout()
{
	std::string session_path = get_session_path();
	DeleteFileA(session_path.c_str());
}

std::string account_manager::get_session_path()
{
	std::string userdata = steam::get_userdata_dir();
	std::string steam_id = steam::get_user_id();
	if (steam_id == "0") {
		ilog(LogLevel::Error, "Couldn't read steam user id, and as a result cannot find the logged in session path!");
		return "";
	}
	std::string game_id = "1276390";
	std::string session_file = userdata + "\\" + steam_id + "\\" + game_id + "\\local\\Data\\Docs\\com.ninjakiwi.link\\Live\\current.session";
	return session_file;
}
