#include "RingBuffer.h"

RingBuffer::RingBuffer() {
	for (size_t i = 0; i < SIZE; ++i) {
		sequences_[i].store(i, std::memory_order_relaxed);
	}
}

bool RingBuffer::enqueue(ClientId clientId, std::string clOrderId, OrderId orderId, Side side, Price price, OrderType type, Quantity initialQuantity,
                         Quantity remainingQuantity, SelfTradePrevention stp) {
	size_t pos = writeIdx_.load(std::memory_order_relaxed);

	while (true) {
		size_t index = pos & mask_;
		size_t seq = sequences_[index].load(std::memory_order_acquire);
		intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);

		if (diff == 0) {
			if (writeIdx_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
				buffer_[index] = OrderRequest(clientId, clOrderId, orderId, side, price, type, initialQuantity, remainingQuantity, stp);
				sequences_[index].store(pos + 1, std::memory_order_release);
				return true;
			}
		} else if (diff < 0) {
			return false;
		} else {
			std::this_thread::yield();
			pos = writeIdx_.load(std::memory_order_relaxed);
		}
	}
}

std::unique_ptr<OrderRequest> RingBuffer::dequeue() {
	size_t pos = readIdx_;
	size_t index = pos & mask_;
	size_t seq = sequences_[index].load(std::memory_order_acquire);
	intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);

	if (diff == 0) {
		std::unique_ptr<OrderRequest> orderToProcess = std::make_unique<OrderRequest>(std::move(buffer_[index]));
		buffer_[index] = OrderRequest();
		sequences_[index].store(pos + SIZE, std::memory_order_release);
		++readIdx_;
		return orderToProcess;
	}
	return nullptr;
}