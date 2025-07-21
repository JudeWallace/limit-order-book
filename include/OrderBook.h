#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "MatchResult.h"
#include "OrderNode.h"
#include "OrderStatus.h"
#include "OrderType.h"
#include "PriceLevel.h"
#include "SelfTradePrevention.h"
#include "Side.h"
#include "Using.h"

class OrderBook {
  private:
	Bids bids_;
	Asks asks_;
	std::unordered_map<OrderId, std::shared_ptr<OrderNode>> allOrders_;
	std::atomic<OrderId> internalId_{0};

	// OrderBookBroadcaster broadcaster_; // placeholder

  public:
	OrderBook() = default;

	void addOrder(ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side, Quantity orderQuantity);
	void addOrder(ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side, Price price, Quantity orderQuantity);
	void cancelOrder(OrderId internalId);
	void modifyOrder(const std::shared_ptr<OrderNode> &order);
	void printLevels() const;
	MatchResult matchOrders(const std::shared_ptr<OrderNode> &order);
	SelfTradeResult resolveSelfTrade(const std::shared_ptr<OrderNode> &takingOrder, const std::shared_ptr<OrderNode> &restingOrder);
	bool canFullyFillFOK(const std::shared_ptr<OrderNode> &order) const;

	~OrderBook() = default;
};
