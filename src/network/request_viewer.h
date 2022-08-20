#ifndef SRC_NETWORK_REQUEST_VIEWER
#define SRC_NETWORK_REQUEST_VIEWER

#include "../render/renderable.h"
#include <transaction.h>
#include <NKModule.h>
#include <nuv.h>
#include <CCurlRequestManager.h>

struct RequestSnapshot {
	std::string requestUrl;
	std::string requestData;
	std::string responseBody;
};

void onRequest(CCurlRequestManager* pCCurlRequestManager, HttpRequestInfo* requestInfo, HttpCallbackFunctor* pCallbackFunctor);
void onResponse(LiNK::NKModule*, HttpRequestInfo*);

struct request_viewer : public renderable {
	request_viewer();
	void on_render() override;
};

#endif /* SRC_NETWORK_REQUEST_VIEWER */
