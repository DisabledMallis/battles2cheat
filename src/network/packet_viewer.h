#ifndef SRC_NETWORK_PACKET_VIEWER
#define SRC_NETWORK_PACKET_VIEWER

#include "../render/renderable.h"
#include <network_queue.h>

struct PacketMeta {
	int actionID;

	PacketMeta(int actionID) {
		this->actionID = actionID;
	}
};

#define MESSAGE_SIZE 2048
struct PacketSnapshot {
	char network_message[MESSAGE_SIZE];
	char input_event[MESSAGE_SIZE];
};

void onPacket(CLegacyNetworkQueue* pNetworkQueue, CNetworkMessage* pNetworkMessage, SInputEvent** ppSInputEvent, void* param_4);

struct packet_viewer : public renderable {
	packet_viewer();
	void on_render() override;
};

#endif /* SRC_NETWORK_PACKET_VIEWER */
