#include "Server.h"

Server::Server() : next_id_(0) {
	setupRoutes();
}

void Server::run() {
	std::thread server_thread([this]() { app_.port(7070).multithreaded().run(); });

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		for (auto &pair : connections_) {
			auto *conn = pair.second;
			if (conn) {
				conn->send_text("Heartbeat from server");
			}
		}
	}

	server_thread.join();
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
	    .onmessage([](crow::websocket::connection &conn, const std::string &msg, bool) { conn.send_text("Echo: " + msg); })
	    .onclose([](crow::websocket::connection &conn, const std::string &reason, uint16_t) { std::cout << "WebSocket closed: " << reason << std::endl; });
}