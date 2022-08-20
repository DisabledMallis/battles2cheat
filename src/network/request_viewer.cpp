#include "request_viewer.h"
#include <vector>
#include <map>
#include <CCurlRequestManager.h>
#include <NKModule.h>
#include <json/json.hpp>
#include <logger.h>
#include <chrono>
#include <mutex>

bool snapshot_requests;

std::unordered_map<std::string, RequestSnapshot*> snapshots;
std::timed_mutex requestLock;
void onRequest(CCurlRequestManager* pCCurlRequestManager, HttpRequestInfo* requestInfo, HttpCallbackFunctor* pCallbackFunctor) {
	if (snapshot_requests) {
		RequestSnapshot* snapshot = new RequestSnapshot();
		snapshot->requestUrl = requestInfo->requestUrl.getString();
		snapshot->requestData = requestInfo->requestBody.getString();
		snapshots[requestInfo->requestBody.getString()] = snapshot;
	}
}

void onResponse(LiNK::NKModule* nkModule, HttpRequestInfo* requestInfo)
{
	if (!requestInfo) { return; };
	if (snapshot_requests) {
		std::string request = requestInfo->requestBody.getString();
		char* response = requestInfo->responseBody;
		if (response) {
			snapshots[request]->responseBody = response;
		}
		else {
			snapshots[request]->responseBody = "No response?";
		}
	}
}

request_viewer::request_viewer() : renderable::renderable("request_viewer")
{

}

void request_viewer::on_render()
{
	ImGui::Begin("Request Viewer");
	ImGui::Checkbox("snapshot requests", &snapshot_requests);
	ImGui::Text("Request Snapshots");
	ImGui::Separator();
	for (const auto& [key, snapshot] : snapshots) {
		ImGui::PushID(snapshot);
		int titleSize = snprintf(nullptr, 0, "Request %s", snapshot->requestUrl.c_str())+1;
		char* titleBuffer = (char*)_malloca(titleSize);
		sprintf_s(titleBuffer, titleSize, "Request %s", snapshot->requestUrl.c_str());
		if (ImGui::CollapsingHeader(titleBuffer)) {
			ImGui::Text("Url: %s", snapshot->requestUrl.c_str());
			ImGui::Text("Data: %s", snapshot->requestData.c_str());
			ImGui::Text("Response: %s", snapshot->responseBody.c_str());
		}
		ImGui::PopID();
	}
	ImGui::End();
}
