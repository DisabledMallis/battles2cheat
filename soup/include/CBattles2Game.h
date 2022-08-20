#ifndef SRC_CBATTLES2GAME
#define SRC_CBATTLES2GAME

#include "base_pointers.h"
#include <nuv.h>

class TransactionFuture
{
public:
	char pad_0000[14056]; //0x0000
	nuv::string next_transaction_uuid; //0x36E8
	nuv::string next_transaction_key; //0x3708
}; //Size: 0x4020

class CBattles2Game : public base_pointers
{
public:
	char pad_00D8[488]; //0x00D8
	TransactionFuture* pTransFuture; //0x02C0
	char pad_02C8[312]; //0x02C8
	nuv::string device_name; //0x0400
	char pad_0420[56]; //0x0420
}; //Size: 0x0458
#endif // !SRC_CBATTLES2GAME
