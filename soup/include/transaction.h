#ifndef SRC_TRANSACTION
#define SRC_TRANSACTION

#include <nuv.h>
#include <vector>
#include <cstddef>
#include <variant>

/*
union transaction_value {
	nuv::string s_value;
	int i_value;
	void* u_value;
};
*/

enum class transaction_type {
	STRING,
	UNKNOWN1,
	INT,
	LONG,
	UNKNOWN4,
	BOOL,
	OBJECT,
	ARRAY
};

static_assert(sizeof(transaction_type) == 4, "transaction_type is not the size of 4 bytes! This will cause a misaligned struct!");

class transaction
{
public:
	nuv::string action; //0x0000
	nuv::string path; //0x0020
	transaction_type val_type; //0x0040
	int32_t N00005478; //0x0044
	union //0x0000
	{
		char bytes_value[0x20];
		nuv::string string_value; //0x0000
		long long long_value;
		int32_t int_value; //0x0000
		bool bool_value; //0x0000
	} value; //0x0048
	static_assert(sizeof(value) == 0x20, "transaction::value is the wrong size");
	nuv::string metadata; //0x0068

public:
	~transaction() = delete;
	const std::variant<bool, int, long long, nuv::string> getValue() {
		switch (val_type) {
		case transaction_type::BOOL:
			return value.bool_value;
		case transaction_type::LONG:
			return value.long_value;
		case transaction_type::STRING:
		case transaction_type::OBJECT:
		case transaction_type::ARRAY:
			return value.string_value;
		case transaction_type::INT:
		default:
			return value.int_value;

		}
	}
}; //Size: 0x0088

class transactionData
{
public:
	bool hasUUID; //0x0000
	char pad_0001[7]; //0x0001
	nuv::string uuid; //0x0008
	nuv::vector<transaction> transactions; //0x0028
	nuv::string filename; //0x0040
	bool hasReason; //0x0060
	char pad_0061[7]; //0x0061
	nuv::string reason; //0x0068
	bool hasKey; //0x0088
	char pad_0089[7]; //0x0089
	nuv::string transaction_key; //0x0090
}; //Size: 0x00B0

static_assert(sizeof(transaction) == 0x88, "transaction is the wrong size!");
static_assert(sizeof(nuv::vector<transaction>) == 0x18, "nuv::vector<transaction> is the wrong size!");

#endif // !SRC_TRANSACTION