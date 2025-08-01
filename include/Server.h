#include <atomic>
#include <chrono>
#include <crow/app.h>
#include <crow/websocket.h>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

#include "RingBuffer.h"

using Id = uint32_t;

class Server {
	private:
		crow::SimpleApp app_;

		std::atomic<Id> next_id_{0};
		std::unordered_map<std::string, crow::websocket::connection *> connections_;
		RingBuffer &messagingQueue_;

		void setupRoutes();

	public:
		Server(RingBuffer &messagingQueue);
		void run();
};