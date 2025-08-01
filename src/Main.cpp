#include "OrderBook.h"
#include "RingBuffer.h"
#include "Server.h"

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

int main() {
	OrderBook ob;
	RingBuffer messagingQueue;
	Server server(messagingQueue);

	std::atomic<bool> running{true};
	std::atomic<int> globalCount{0};

	const int totalOrders = 100;
	const int numProducers = 4;
	const int ordersPerProducer = totalOrders / numProducers;

	std::vector<std::thread> producerThreads;

	for (int i = 0; i < numProducers; ++i) {
		producerThreads.emplace_back([i, &messagingQueue, &running, &globalCount, ordersPerProducer]() {
			int localCount = 0;
			while (running && localCount < ordersPerProducer) {
				int count = globalCount.fetch_add(1);
				ClientId clientId = count;
				std::string clOrderId = "Order_" + std::to_string(count);
				OrderId orderId = count;
				Side side = Side::BUY;
				Price price = 100 + count;
				OrderType type = OrderType::Limit;
				Quantity initialQuantity = 10;
				Quantity remainingQuantity = 10;
				SelfTradePrevention stp = SelfTradePrevention::RTO;

				bool enqueued = messagingQueue.enqueue(clientId, clOrderId, orderId, side, price, type, initialQuantity, remainingQuantity, stp);
				if (enqueued) {
					++localCount;
				} else {
					std::cout << "[Producer " << i << "] Ring buffer full, retrying..." << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		});
	}

	std::thread processOrderRequests([&ob, &messagingQueue, &running]() {
		std::cout << "Processing order requests from RingBuffer...\n";
		while (running) {
			if (auto ptr = messagingQueue.dequeue()) {
				ob.addOrder(ptr->orderId_, ptr->clientId_, ptr->clOrderId_, ptr->type_, ptr->side_, ptr->price_, ptr->initialQuantity_);
			} else {
				std::this_thread::yield();
			}
		}
	});

	for (auto &thread : producerThreads) {
		thread.join();
	}

	running = false;
	processOrderRequests.join();

	ob.printLevels();

	std::cout << "Test complete.\n";
	return 0;
}
