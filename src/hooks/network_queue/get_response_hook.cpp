#include "get_response_hook.h"

//40 ?? 53 56 57 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? 49

#include <memory.h>
#include <logger.h>
#include "../../network/request_viewer.h"
#include <NKModule.h>
#include <CCurlRequestManager.h>
#include <fstream>
#include <filesystem>

static uint64_t oggetresponse;

void* f_getresponsehook(LiNK::NKModule* nkModule, HttpRequestInfo* requestInfo) {
#ifdef _DEBUG
	ilog(LogLevel::Warning, "Receiving request response");
	ilog("URL: %s", requestInfo->requestUrl.c_str());
	ilog("Body: %s", requestInfo->requestBody.c_str());
	ilog("Response: %s", requestInfo->responseBody);
	ilog("End of response");

	std::filesystem::path cd = std::filesystem::current_path();
	std::filesystem::path requestLog = cd / "requestLogs" / "requests.log";
	std::filesystem::create_directories(requestLog.parent_path());
	std::fstream logStream(requestLog, std::ios::app);
	logStream << "BEGIN DOWN\n";
	logStream << requestInfo->requestUrl.getString() + "\n";
	logStream << requestInfo->requestBody.getString() + "\n";
	if (requestInfo->responseBody) {
		logStream << std::string(requestInfo->responseBody) + "\n";
	}
	else {
		logStream << "(null)\n";
	}
	logStream << "END DOWN\n\n";
	logStream.close();
#endif
	return PLH::FnCast(oggetresponse, f_getresponsehook)(nkModule, requestInfo);
}

auto network_queue::getresponse_hook::apply() -> bool {
	//?? 89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? 4C 8B ?? ?? 89 ?? ?? 45
	uintptr_t fp = (uintptr_t)(memory::FindSig("89 ?? ?? ?? 55 56 57 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? 4C 8B ?? ?? 89 ?? ?? 45")-1);

	this->create_detour(fp, (uint64_t)&f_getresponsehook, (uint64_t*)&oggetresponse);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto network_queue::getresponse_hook::remove() -> bool {
	return this->get_detour()->unHook();
}