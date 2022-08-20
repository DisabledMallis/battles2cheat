#include "send_request_hook.h"
#include <memory.h>
#include "../../network/request_viewer.h"
#include <json/json.hpp>
#include <CCurlRequestManager.h>
#include <logger.h>
#include <files.h>
#include "../../network/request_viewer.h"

static uint64_t ogsendrequest;
static size_t requestCount = 0;

size_t blockedRequests = 0;
extern bool bp1;
extern bool bp2;
extern bool bp3;

void* f_sendrequesthook(CCurlRequestManager* pCCurlRequestManager, HttpRequestInfo* requestInfo, HttpCallbackFunctor* pCallbackFunctor) {
#ifdef _DEBUG
	ilog(LogLevel::Warning, "Sending request");
	ilog("URL: %s", requestInfo->requestUrl.c_str());
	ilog("Body: %s", requestInfo->requestBody.c_str());
	ilog("End of send");
#endif
	if (bp1) {
		if (requestInfo->requestUrl.getString().find("bfb.ninjakiwi") != std::string::npos) {
			if (requestInfo->requestBody.getString().find("battles2/mobile/battlePressed") != std::string::npos) {
				ilog("Queue Request, allowing...");
				return PLH::FnCast(ogsendrequest, f_sendrequesthook)(pCCurlRequestManager, requestInfo, pCallbackFunctor);
			}
			ilog("Blocking bannable request");
			ilog("Blocked URL: %s", requestInfo->requestUrl.c_str());
			ilog("Blocked Body: %s", requestInfo->requestBody.c_str());
			blockedRequests++;
			return (void*)true;
		}
	}
#ifdef _DEBUG
	std::filesystem::path cd = std::filesystem::current_path();
	std::filesystem::path requestLog = cd / "requestLogs" / "requests.log";
	std::filesystem::create_directories(requestLog.parent_path());
	std::fstream logStream(requestLog, std::ios::app);
	logStream << "BEGIN UP\n";
	logStream << requestInfo->requestUrl.getString() + "\n";
	logStream << requestInfo->requestBody.getString() + "\n";
	logStream << "END UP\n\n";
	logStream.close();
#endif
	/*
	if (bp3) {
		if (requestInfo->requestBody.getString().find("Update loadout from started game") != std::string::npos) {
			ilog("Blocked loadout update");
			return (void*)true;
		}
	}
	*/
	return PLH::FnCast(ogsendrequest, f_sendrequesthook)(pCCurlRequestManager, requestInfo, pCallbackFunctor);
}

auto network_queue::sendrequest_hook::apply() -> bool {
	uintptr_t fp = (uintptr_t)(memory::FindSig("89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? ?? 49 8B ?? 48 8B ?? 48")-1);

	this->create_detour(fp, (uint64_t)&f_sendrequesthook, (uint64_t*)&ogsendrequest);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto network_queue::sendrequest_hook::remove() -> bool {
	return this->get_detour()->unHook();
}