#pragma once

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>

#include "MatchResult.h"
#include "OrderNode.h"
#include "OrderStatus.h"
#include "OrderType.h"
#include "PriceLevel.h"
#include "SelfTradePrevention.h"
#include "Side.h"
#include "Using.h"
#include "Constants.h"

class OrderBook {
	private:
		Bids bids_;
		Asks asks_;
		Price midPrice_ = Constants::InvalidPrice;
		Price bestBid_ = Constants::InvalidPrice;
		Price bestAsk_ = Constants::InvalidPrice;
		Price lastTradedPrice_ = Constants::InvalidPrice;
		std::unordered_map<OrderId, std::shared_ptr<OrderNode>> allOrders_;
		MatchResult matchOrders(const std::shared_ptr<OrderNode> &order);
		SelfTradeResult resolveSelfTrade(const std::shared_ptr<OrderNode> &takingOrder, const std::shared_ptr<OrderNode> &restingOrder);
		bool canFullyFillFOK(const std::shared_ptr<OrderNode> &order) const;

	public:
		OrderBook() = default;

		void addOrder(OrderId internalId, ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side,
					Quantity orderQuantity);
		void addOrder(OrderId internalId, ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side, Price price,
					Quantity orderQuantity);
		void cancelOrder(OrderId internalId);
		void modifyOrder(const std::shared_ptr<OrderNode> &order);
		void updateMarketState();
		
		const Bids& getBids() { return bids_; }
		const Asks& getAsks() { return asks_; }
		Price getBestBid() const { return bestBid_; }
		Price getBestAsk() const { return bestAsk_; }
		Price getMidPrice() const { return midPrice_; }
		Price getLastTradedPrice() const { return lastTradedPrice_; }

		void printLevels() const;

		~OrderBook() = default;
};
