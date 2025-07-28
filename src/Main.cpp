#include "OrderBook.h"
#include "RingBuffer.h"
#include "Server.h"

#include <chrono>
#include <fstream>
#include <thread>

int main() {
	OrderBook ob;
	RingBuffer messagingQueue;
	Server server(messagingQueue);

	std::thread serverThread([&server]() { server.run(); });

	std::ofstream logFile("log.txt", std::ios::app);
	if (!logFile.is_open()) {
		std::cerr << "Failed to open log file!" << std::endl;
		return 1;
	}

	std::thread processOrderRequests([&ob, &messagingQueue, &logFile]() {
		while (true) {
			if (messagingQueue.dequeue()) {
				logFile << "Processing order request from RingBuffer." << std::endl;
				logFile.flush();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});

	serverThread.join();
	processOrderRequests.join();

	return 0;
}