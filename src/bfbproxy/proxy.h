#ifndef SRC_BFBPROXY_PROXY
#define SRC_BFBPROXY_PROXY

#include <string>

//#define USE_HTTPS

namespace bfb_proxy {
	void send_analytics();
	void start_server();
	void stop_server();
};
#endif