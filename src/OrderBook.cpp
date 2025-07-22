#include "OrderBook.h"

void OrderBook::addOrder(ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side, Quantity orderQuantity) {
	auto thisOrderId = ++internalId_;

	if ((side == Side::BUY && !asks_.empty()) || (side == Side::SELL && !bids_.empty())) {
		auto order = std::make_shared<OrderNode>(clId, externalClientOrderId, thisOrderId, orderType, side, orderQuantity);

		auto outcome = matchOrders(order);
		std::cout << outcome.status << " for orderId: " << order->orderId << std::endl;
		return;
	}
	// log order cancelled as of no liquidity
	// std::cout << "No liquidity for order: " << thisOrderId << std::endl;
}

void OrderBook::addOrder(ClientId clId, const std::string &externalClientOrderId, OrderType orderType, Side side, Price price, Quantity orderQuantity) {
	auto thisOrderId = ++internalId_;
	auto order = std::make_shared<OrderNode>(clId, externalClientOrderId, thisOrderId, orderType, side, price, orderQuantity);

	auto outcome = matchOrders(order);
	std::cout << outcome.status << " for orderId: " << order->orderId << std::endl;

	if (outcome.status == OrderStatus::Cancelled) {
		return;
	}

	// std::cout << result.status << std::endl;

	if (order->remainingQuantity > 0) {
		if (order->side == Side::BUY) {
			bids_[order->price].addOrder(order);
		} else {
			asks_[order->price].addOrder(order);
		}

		allOrders_[thisOrderId] = order;
		outcome.status = OrderStatus::PlacedOrder;
		std::cout << outcome.status << " for orderId: " << order->orderId << std::endl;
	}
}

void OrderBook::modifyOrder(const std::shared_ptr<OrderNode> &order) {
	/*
	std::string clOrderId = order->clOrderId;
	cancelOrder(order->orderId);
	*/
}

void OrderBook::cancelOrder(OrderId internalId) {
	auto isOrderPresent = allOrders_.find(internalId);

	if (isOrderPresent == allOrders_.end())
		return;

	auto &order = allOrders_[internalId];

	if (order->side == Side::BUY) {
		bids_[order->price].removeOrder(order);
		if (bids_[order->price].isEmpty()) {
			bids_.erase(order->price);
		}
	} else {
		asks_[order->price].removeOrder(order);
		if (asks_[order->price].isEmpty()) {
			asks_.erase(order->price);
		}
	}

	allOrders_.erase(internalId);
	std::cout << "Cancelled order: " << internalId << std::endl;
}

SelfTradeResult OrderBook::resolveSelfTrade(const std::shared_ptr<OrderNode> &takingOrder, const std::shared_ptr<OrderNode> &restingOrder) {
	switch (takingOrder->stp) {
		case SelfTradePrevention::RTO:
			return SelfTradeResult::Cancelled;
		case SelfTradePrevention::RRO:
			// publish message
			cancelOrder(restingOrder->orderId);
			return SelfTradeResult::Continue;
		case SelfTradePrevention::RBO:
			// publish message
			cancelOrder(restingOrder->orderId);
			return SelfTradeResult::Cancelled;
	}
	return SelfTradeResult::Error;
}

bool OrderBook::canFullyFillFOK(const std::shared_ptr<OrderNode> &order) const {
	Quantity requiredLiquidtiy = order->initialQuantity;
	Quantity restingQuantity = 0;

	switch (order->side) {
		case Side::BUY:
			for (auto it = asks_.begin(); it != asks_.end(); it++) {
				if (it->first <= order->price) {
					restingQuantity += it->second.levelQuantity_;
				} else {
					break;
				}
			}

			if (requiredLiquidtiy > restingQuantity)
				return false;

			restingQuantity = 0;

			for (auto it = asks_.begin(); it != asks_.end(); it++) {
				auto restingOrder = it->second.peek();
				while (restingOrder) {
					if ((restingOrder->clientId == order->clientId) && order->stp != SelfTradePrevention::RRO) {
						return false;
					} else if (restingOrder->clientId != order->clientId) {
						restingQuantity += restingOrder->remainingQuantity;
					}

					if (restingQuantity >= requiredLiquidtiy)
						return true;

					restingOrder = restingOrder->nextOrder;
				}
			}
			break;
		case Side::SELL:
			for (auto it = bids_.begin(); it != bids_.end(); it++) {
				if (it->first >= order->price) {
					restingQuantity += it->second.levelQuantity_;
				} else {
					break;
				}
			}

			if (requiredLiquidtiy > restingQuantity)
				return false;

			restingQuantity = 0;

			for (auto it = bids_.begin(); it != bids_.end(); it++) {
				auto restingOrder = it->second.peek();
				while (restingOrder) {
					if ((restingOrder->clientId == order->clientId) && order->stp != SelfTradePrevention::RRO) {
						return false;
					} else if (restingOrder->clientId != order->clientId) {
						restingQuantity += restingOrder->remainingQuantity;
					}

					if (restingQuantity >= requiredLiquidtiy)
						return true;

					restingOrder = restingOrder->nextOrder;
				}
			}
			break;
		default:
			break;
	}

	return false;
}

MatchResult OrderBook::matchOrders(const std::shared_ptr<OrderNode> &order) {
	MatchResult tradeResult;

	if (order->type == OrderType::FillOrKill) {
		if (!canFullyFillFOK(order)) {
			tradeResult.status = OrderStatus::Cancelled;
			return tradeResult;
		}
	}

	if (order->side == Side::BUY) {

		while (order->remainingQuantity > 0 && !asks_.empty()) {
			auto it = asks_.begin();
			Price bestAsk = it->first;

			if (order->type == OrderType::Limit && order->price < bestAsk) {
				// handle correctly
				return tradeResult;
			}

			auto &level = it->second;
			while (!level.isEmpty() && order->remainingQuantity > 0) {
				auto topAskOrder = level.peek();

				// test and resolve for self trade
				if (order->clientId == topAskOrder->clientId) {
					if (resolveSelfTrade(order, topAskOrder) == SelfTradeResult::Continue) {
						continue;
					} else {
						tradeResult.status = OrderStatus::SelfTradePrevention;
						return tradeResult;
					}
				}

				Quantity matched = std::min(order->remainingQuantity, topAskOrder->remainingQuantity);
				order->remainingQuantity -= matched;
				topAskOrder->remainingQuantity -= matched;
				level.levelQuantity_ -= matched;

				if (topAskOrder->remainingQuantity == 0) {
					level.pop();
					allOrders_.erase(topAskOrder->orderId);
				}
			}

			if (level.isEmpty()) {
				asks_.erase(bestAsk);
			}
		}
	} else {
		while (order->remainingQuantity > 0 && !bids_.empty()) {
			auto it = bids_.begin();
			Price bestBid = it->first;

			if (order->type == OrderType::Limit && order->price > bestBid)
				// NoLiquidity comes from here
				return tradeResult;

			auto &level = it->second;
			while (!level.isEmpty() && order->remainingQuantity > 0) {
				auto topBidOrder = level.peek();

				if (order->clientId == topBidOrder->clientId) {
					if (resolveSelfTrade(order, topBidOrder) == SelfTradeResult::Continue) {
						continue;
					} else {
						tradeResult.status = OrderStatus::SelfTradePrevention;
						return tradeResult;
					}
				}

				Quantity matched = std::min(order->remainingQuantity, topBidOrder->remainingQuantity);
				order->remainingQuantity -= matched;
				topBidOrder->remainingQuantity -= matched;
				level.levelQuantity_ -= matched;

				if (topBidOrder->remainingQuantity == 0) {
					level.pop();
					allOrders_.erase(topBidOrder->orderId);
				}
			}

			if (level.isEmpty()) {
				bids_.erase(bestBid);
			}
		}
	}

	if (order->remainingQuantity == 0) {
		tradeResult.status = OrderStatus::Filled;
	} else if (order->remainingQuantity > 0 && order->remainingQuantity < order->initialQuantity) {
		tradeResult.status = OrderStatus::PartiallyFilled;
	}

	return tradeResult;
}

void OrderBook::printLevels() const {
	std::cout << "Bids: " << std::endl;
	for (auto &[price, level] : bids_) {
		std::cout << price << " " << level.levelQuantity_ << ": ";
		level.printOrders();
	}

	std::cout << "Asks: " << std::endl;
	for (auto &[price, level] : asks_) {
		std::cout << price << " " << level.levelQuantity_ << ": ";
		level.printOrders();
	}
}