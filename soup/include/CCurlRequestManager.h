#ifndef SRC_CCURLREQUESTMANAGER
#define SRC_CCURLREQUESTMANAGER

#include <nuv.h>

struct HttpCallbackFunctor
{
	char pad_0000[16]; //0x0000
	void(**lambdaCallback)(); //0x0010
	char pad_0018[240]; //0x0018
}; //Size: 0x0108

struct HttpRequestInfo
{
	nuv::string gameInfo; //0x0000
	char pad_0020[32]; //0x0020
	nuv::string requestUrl; //0x0040
	int32_t dunno; //0x0060
	int32_t dunno2; //0x0064
	int32_t dunno3; //0x0068
	int32_t dunno4; //0x006C
	char pad_0070[72]; //0x0070
	class HttpCallbackFunctor* pCallbackFunctor; //0x00B8
	char* responseBody; //0x00C0
	char pad_00C8[32]; //0x00C8
	nuv::string requestBody; //0x00E8
	class CurlRequestData* pCurlRequestData; //0x0108
	char pad_0110[8]; //0x0110
	nuv::string contentType; //0x0118
	char pad_0138[8]; //0x0138
}; //Size: 0x05F8

struct CCurlRequestManager {
};

#endif // !SRC_CCURLREQUESTMANAGER
