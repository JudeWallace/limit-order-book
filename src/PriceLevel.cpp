#include "PriceLevel.h"

void PriceLevel::addOrder(const std::shared_ptr<OrderNode> &order) {
	if (!head_) {
		head_ = tail_ = order;
	} else {
		tail_->nextOrder = order;
		order->prevOrder = tail_;
		tail_ = order;
	}

	levelQuantity_ += order->remainingQuantity;
}

void PriceLevel::removeOrder(const std::shared_ptr<OrderNode> &order) {
	auto prev = order->prevOrder.lock();
	auto next = order->nextOrder;

	if (order == head_) {
		head_ = next;
		if (head_) {
			head_->prevOrder.reset();
		} else {
			tail_.reset();
		}
	} else if (order == tail_) {
		tail_ = prev;
		if (tail_) {
			tail_->nextOrder.reset();
		}
	} else {
		if (prev) {
			prev->nextOrder = next;
		}
		if (next) {
			next->prevOrder = prev;
		}
	}

	order->prevOrder.reset();
	order->nextOrder.reset();

	levelQuantity_ -= order->remainingQuantity;
}

std::shared_ptr<OrderNode> PriceLevel::pop() {
	if (!isEmpty()) {
		auto top = head_;
		head_ = head_->nextOrder;

		if (head_) {
			head_->prevOrder.reset();
		} else {
			tail_.reset();
		}

		top->nextOrder = nullptr;
		return top;
	}

	return nullptr;
}

std::shared_ptr<OrderNode> PriceLevel::peek() const { return head_; }

void PriceLevel::printOrders() const {
	auto current = head_;
	while (current) {
		std::cout << "OrderId: " << current->orderId << " ";
		current = current->nextOrder;
	}
	std::cout << std::endl;
}

bool PriceLevel::isEmpty() const { return head_ == nullptr; }