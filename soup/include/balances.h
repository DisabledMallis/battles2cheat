#ifndef SRC_BALANCES
#define SRC_BALANCES

#include <nuv.h>

class parsed_balances
{
public:
	nuv::string nk_api_id;
	nuv::string wallet_name; //0x0020
	char* currencies; //0x0040
}; //Size: 0x0048

#endif