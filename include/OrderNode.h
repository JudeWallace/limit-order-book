#pragma once

#include <memory>
#include <string>

#include "OrderType.h"
#include "SelfTradePrevention.h"
#include "Side.h"
#include "Using.h"

struct OrderNode {
	ClientId clientId;
	std::string clOrderId;
	OrderId orderId;
	Side side;
	Price price;
	OrderType type;
	Quantity initialQuantity;
	Quantity remainingQuantity;

	SelfTradePrevention stp = SelfTradePrevention::RTO;

	std::weak_ptr<OrderNode> prevOrder;
	std::shared_ptr<OrderNode> nextOrder = nullptr;

	OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Price p, Quantity quantity);
	OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Quantity quantity);

	~OrderNode() = default;
};