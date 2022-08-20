#ifndef INJECTOR_DGDATA
#define INJECTOR_DGDATA

#include <string>
#include <vector>
#include "../json/json.hpp"

namespace dgdata {
	struct raw_dgdata {
		std::vector<char> data;
		raw_dgdata();
		raw_dgdata(char* dgdata, size_t data_size);
		nlohmann::json decode();
	};
	nlohmann::json decode(char* dgdata, size_t data_size);
	//std::string encode(std::string json_text);

	void to_json(nlohmann::json& j, const raw_dgdata& p);

	void from_json(const nlohmann::json& j, raw_dgdata& p);
};

#endif // !INJECTOR_DGDATA
