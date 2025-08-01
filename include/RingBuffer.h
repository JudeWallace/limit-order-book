#pragma once

#include "OrderRequest.h"
#include "SelfTradePrevention.h"
#include "Using.h"

#include <atomic>
#include <memory>
#include <string>
#include <thread>

constexpr size_t SIZE = 1024;

class RingBuffer {
	private:
		std::array<OrderRequest, SIZE> buffer_;
		std::array<std::atomic<size_t>, SIZE> sequences_;
		std::atomic<size_t> writeIdx_{0};
		size_t readIdx_{0};
		const size_t mask_ = SIZE - 1;
		std::atomic<OrderId> nextOrderId_{1};

	public:
		RingBuffer();

		bool enqueue(ClientId clientId, std::string clOrderId, OrderId orderId, Side side, Price price, OrderType type, Quantity initialQuantity,
					Quantity remainingQuantity, SelfTradePrevention stp);
		std::unique_ptr<OrderRequest> dequeue();
};
