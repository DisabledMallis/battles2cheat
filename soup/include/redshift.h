#ifndef SRC_REDSHIFT
#define SRC_REDSHIFT

#include <nuv.h>

class DGAnalytics_Redshift
{
public:
	char pad_0000[80]; //0x0000
	nuv::string sEventsLogging; //0x0050
	nuv::string url; //0x0070
	char pad_0090[216]; //0x0090
	class CCurlRequestManager* pCCurlHttpRequestManager; //0x0168
	char pad_0170[16]; //0x0170
	nuv::string request_url; //0x0180
	nuv::string game; //0x01A0
	nuv::string source; //0x01C0
	char pad_01E0[24]; //0x01E0
	int64_t N00000516; //0x01F8
	int64_t N00000517; //0x0200
	int64_t N00000518; //0x0208
	char pad_0210[384]; //0x0210
}; //Size: 0x0390

#endif // !SRC_REDSHIFT
