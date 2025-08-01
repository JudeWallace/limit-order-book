#pragma once

#include <iostream>
#include <map>
#include <memory>

#include "OrderNode.h"

struct PriceLevel {
	private:
		std::shared_ptr<OrderNode> head_;
		std::shared_ptr<OrderNode> tail_;

	public:
		Quantity levelQuantity_ = 0;

		PriceLevel() = default;

		void addOrder(const std::shared_ptr<OrderNode> &order);
		void removeOrder(const std::shared_ptr<OrderNode> &order);
		std::shared_ptr<OrderNode> pop();
		std::shared_ptr<OrderNode> peek() const;
		void printOrders() const;
		bool isEmpty() const;
	};

using Bids = std::map<Price, PriceLevel, std::greater<Price>>;
using Asks = std::map<Price, PriceLevel, std::less<Price>>;