#include "Server.h"

Server::Server(RingBuffer &ringBuffer) : next_id_(0), messagingQueue_(ringBuffer) {
	setupRoutes();
}

void Server::run() {
	app_.port(7070).multithreaded().run();
}

void Server::setupRoutes() {
	CROW_ROUTE(app_, "/")([]() { return "Hello, from limit-order-book backend!"; });

	CROW_ROUTE(app_, "/ws")
	    .websocket(&app_)
	    .onopen([this](crow::websocket::connection &conn) {
		    std::cout << "WebSocket connected.\n";
		    uint32_t clientId = ++next_id_;
		    connections_[std::to_string(clientId)] = &conn;
		    conn.send_text("Hello, from your WebSocket connection!");
	    })
	    .onmessage([this](crow::websocket::connection &conn, const std::string &msg, bool) {
		    /*
		            bool added = messagingQueue_.enqueue();

		            if (added) {
		                conn.send_text("Order request added to the queue.");
		            } else {
		                conn.send_text("Queue is full, please try again later.");
		            }
		            */
	    })
	    .onclose(
	        [](crow::websocket::connection &conn, const std::string &reason, uint16_t) { std::cout << "WebSocket closed: " << reason << std::endl; });
}