#include "dgdata.h"
#include "../logger.h"
#include <Windows.h>

nlohmann::json dgdata::decode(char* dgdata, size_t data_size) {
	/*
	js_wrapper::setup_runtime();
	js_wrapper::run_script(DGDATA_JS);
	std::wstring result = js_wrapper::run_script("decode(\"" + dgdata_text + "\");");
	std::string s_result = js_wrapper::ws_to_s(result);
	js_wrapper::destroy_runtime();
	return s_result;
	*/
	char* buffer = (char*)malloc(data_size+1);
	if (buffer == nullptr) {
		ilog("Couldn't allocate a buffer for decryption!");
		return "";
	}
	for (size_t i = 0; i < data_size; i++) {
		buffer[i] = dgdata[i];
	}
	//MessageBoxA(nullptr, buffer, "", MB_OK);
	if (data_size < 15) {
		//Bad things are happenining
		return nlohmann::json();
	}
	for (int i = 14; i < data_size; i++) {
		buffer[i] -= (char)(21 + (i - 14) % 6);
	}

	buffer[data_size] = 0;

	std::string decrypted = std::string(buffer).substr(14);
	free(buffer);

	return nlohmann::json::parse(decrypted);
}
void dgdata::to_json(nlohmann::json& j, const raw_dgdata& p) {
	j = nlohmann::json{ {"data", p.data} };
}
void dgdata::from_json(const nlohmann::json& j, raw_dgdata& p) {
	j.at("data").get_to(p.data);
}

dgdata::raw_dgdata::raw_dgdata()
{
	//throw std::exception("Do not create a raw_dgdata instance without any data!");
}

dgdata::raw_dgdata::raw_dgdata(char* dgdata, size_t data_size)
{
	for (size_t i = 0; i < data_size; i++) {
		this->data.push_back(dgdata[i]);
	}
}

nlohmann::json dgdata::raw_dgdata::decode()
{
	std::vector<char> raw_data = this->data;
	char* data_buffer = (char*)malloc(raw_data.size() + 1);
	if (data_buffer == nullptr) {
		return nlohmann::json();
	}
	for (size_t i = 0; i < raw_data.size(); i++) {
		data_buffer[i] = raw_data[i];
	}
	size_t data_size = raw_data.size();
	data_buffer[data_size] = 0;
	nlohmann::json parsed_data = dgdata::decode(data_buffer, data_size);
	free(data_buffer);
	return parsed_data;
}
