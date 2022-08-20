#include "redshift_hook.h"
#include "../../memory.h"

static uint64_t ogredshift;

DGAnalytics_Redshift* p_redshift = nullptr;

//static nuv::string urlCopy = std::string();
//static nuv::string requestUrlCopy = std::string();

//extern bool bp0;
extern bool bp3;
//static bool firstRun = true;
//static bool firstUndo = true;


void f_redshifthook(DGAnalytics_Redshift* redshift_data, uint64_t cProcessAnalyticsQueueEvent, uint64_t idk) {
	p_redshift = redshift_data;

	//TODO: Updated bypass
	if (p_redshift) {
		/*
		if (bp0) {
			if (firstRun) {
				memcpy_s(&urlCopy, sizeof(nuv::string), &p_redshift->url, sizeof(nuv::string));
				memcpy_s(&requestUrlCopy, sizeof(nuv::string), &p_redshift->request_url, sizeof(nuv::string));
				firstRun = false;
			}
			p_redshift->url = "http://127.0.0.1:5743/multi";
			p_redshift->request_url = "http://127.0.0.1:5743/multi";
			firstUndo = true;
		}
		else {
			if (firstUndo) {
				memcpy_s(&p_redshift->url, sizeof(nuv::string), &urlCopy, sizeof(nuv::string));
				memcpy_s(&p_redshift->request_url, sizeof(nuv::string), &requestUrlCopy, sizeof(nuv::string));
				firstUndo = false;
			}
		}
		*/
	}

	/*
	if (bp3) {
		std::string uid = userId->getString();
		if (uid.find("NO_LINK") != std::string::npos) {
			userId->setString("NO_LINK{0}");
		}
	}
	*/
	
	PLH::FnCast(ogredshift, f_redshifthook)(redshift_data, cProcessAnalyticsQueueEvent, idk);
}

auto redshift_hook::apply() -> bool {
	uintptr_t fp = (uintptr_t)(memory::FindSig("89 ?? ?? ?? ?? 89 ?? ?? ?? ?? 89 ?? ?? ?? 55 41 ?? 41 ?? 41 ?? 41 ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 81 ?? ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 ?? ?? 89 ?? ?? ?? ?? ?? 4C 8B ?? ?? B9") - 1);

	this->create_detour(fp, (uint64_t)&f_redshifthook, (uint64_t*)&ogredshift);
	if (this->get_detour()->hook()) {
		return true;
	}
	return false;
}

auto redshift_hook::remove() -> bool {
	return this->get_detour()->unHook();
}