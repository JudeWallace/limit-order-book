#pragma once

#ifdef SelfTradePrevention
#error "SelfTradePrevention is defined as a macro!"
#endif

#include <memory>
#include <string>

#include "SelfTradePrevention.h"

// Test if enum is visible
static_assert(static_cast<int>(SelfTradePrevention::RTO) >= 0, "SelfTradePrevention not properly defined");

#include "OrderType.h"
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

	SelfTradePrevention stpf = SelfTradePrevention::RTO;

	std::weak_ptr<OrderNode> prevOrder;
	std::shared_ptr<OrderNode> nextOrder = nullptr;

	OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Price p, Quantity quantity);
	OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Quantity quantity);

	~OrderNode() = default;
};