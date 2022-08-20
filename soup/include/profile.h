#ifndef SRC_PROFILE
#define SRC_PROFILE

#include <nuv.h>

class user
{
public:
	nuv::string nkapiID;
	nuv::string displayName; //0x0020
	int64_t clan; //0x0040
	nuv::string country; //0x0048
	nuv::string continent; //0x0068
	int64_t avatar; //0x0088
	bool online; //0x0090
	char pad_0091[7]; //0x0091
	int64_t onlineApp; //0x0098
	class std::vector<void*> providers; //0x00A0
	char pad_00B8[16]; //0x00B8
	nuv::string shortcode; //0x00C8
	nuv::string safeName; //0x00E8
}; //Size: 0x0108

class session
{
public:
	nuv::string sessionID;
	int64_t expires; //0x0020
}; //Size: 0x0028

class profile
{
public:
	char pad_0000[8]; //0x0000
	class N000040E8* nk_app; //0x0008
	char pad_0010[128]; //0x0010
	session session; //0x0090
	user user; //0x00B8
	char pad_01C0[152]; //0x01C0
}; //Size: 0x06B8

#endif /* SRC_PROFILE */
