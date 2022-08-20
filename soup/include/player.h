#ifndef SRC_PLAYER
#define SRC_PLAYER

#include <nuv.h>
#include <vector>
#include "enc_val.h"

class no_clue_a
{
public:
	char pad_0000[80]; //0x0000
	void* getsPlayer; //0x0050
	char pad_0058[48]; //0x0058
}; //Size: 0x0108

class no_clue_b
{
public:
	char pad_0000[16]; //0x0000
	class no_clue_a* p_no_clue_a; //0x0010
	char pad_0018[48]; //0x0018
}; //Size: 0x0048

struct player {
	float idkVal; //0x0000
	char pad_0004[4]; //0x0004
	nuv::string username; //0x0008
	nuv::string masked_name; //0x0028
	char pad_0048[1816]; //0x0048
	float cash_decrypted; //0x0760
	float eco_decrypted; //0x0764
	float health_decrypted; //0x0768
	char pad_076C[4]; //0x076C
	float eco_timer_decrypted; //0x0770
	int32_t round_decrypted; //0x0774
	char pad_0778[192]; //0x0778
	class N000002C9* N00000154; //0x0838
	char pad_0840[16]; //0x0840
	class no_clue_b* weirdo; //0x0850
	char pad_0858[200]; //0x0858
	int64_t tower_count; //0x0920
	char pad_0928[56]; //0x0928
	int64_t placed_tower_count; //0x0960
	char pad_0968[1384]; //0x0968
}; //Size: 0x0ED0

static_assert(sizeof(nuv::string) == 0x20, "nuv::string isn't being built safely!");

#endif /* SRC_PLAYER */
