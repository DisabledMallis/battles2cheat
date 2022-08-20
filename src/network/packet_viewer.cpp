#include "packet_viewer.h"
#include <vector>

bool preview_packets;
bool snapshot_packets;

std::vector<PacketMeta*> previews;
std::vector<PacketSnapshot*> snapshots;
void onPacket(CLegacyNetworkQueue* pNetworkQueue, CNetworkMessage* pNetworkMessage, SInputEvent** ppSInputEvent, void* param_4) {
	if (preview_packets) {
		PacketMeta* packet_meta = new PacketMeta((*ppSInputEvent)->actionID);
		previews.push_back(packet_meta);
	}

	if (snapshot_packets) {
		PacketSnapshot* packet_snapshot = new PacketSnapshot();
		memcpy(packet_snapshot->network_message, pNetworkMessage, MESSAGE_SIZE);
		memcpy(packet_snapshot->input_event, *ppSInputEvent, MESSAGE_SIZE);
		snapshots.push_back(packet_snapshot);
	}
}

packet_viewer::packet_viewer() : renderable::renderable("packet_viewer")
{

}

void packet_viewer::on_render()
{
	ImGui::Begin("Packet Viewer");
	ImGui::Checkbox("preview packets", &preview_packets);
	ImGui::Checkbox("snapshot packets", &snapshot_packets);
	if (ImGui::CollapsingHeader("Previews")) {
		ImGui::BeginListBox("Packet Previews");
		for (auto preview : previews) {
			ImGui::Text("Packet of type %x was sent!", preview->actionID);
		}
		ImGui::EndListBox();
	}
	if (ImGui::CollapsingHeader("Snapshots")) {
		ImGui::BeginListBox("Packet Snapshots");
		for (auto snapshot : snapshots) {
			ImGui::PushID(snapshot);
			if (ImGui::CollapsingHeader("Packet")) {
				char msg_loc_buf[64];
				sprintf(msg_loc_buf, "%p", snapshot->network_message);
				ImGui::InputText("Message Snapshot location", msg_loc_buf, 64);

				char input_loc_buf[64];
				sprintf(input_loc_buf, "%p", snapshot->input_event);
				ImGui::InputText("InputEvent Snapshot location", input_loc_buf, 64);
			}
			ImGui::PopID();
		}
		ImGui::EndListBox();
	}
	ImGui::End();
}
