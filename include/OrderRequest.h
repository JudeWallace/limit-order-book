#pragma once

#include "OrderType.h"
#include "SelfTradePrevention.h"
#include "Side.h"
#include "Using.h"

#include <string>

struct OrderRequest {
	ClientId clientId_;
	std::string clOrderId_;
	OrderId orderId_;
	Side side_;
	Price price_;
	OrderType type_;
	Quantity initialQuantity_;
	Quantity remainingQuantity_;
	SelfTradePrevention stp_ = SelfTradePrevention::RTO;

	OrderRequest(ClientId clientId, std::string clOrderId, OrderId orderId, Side side, Price price, OrderType type, Quantity initialQuantity,
	             Quantity remainingQuantity, SelfTradePrevention stp)
	    : clientId_(clientId), clOrderId_(clOrderId), orderId_(orderId), side_(side), price_(price), type_(type), initialQuantity_(initialQuantity),
	      remainingQuantity_(remainingQuantity), stp_(stp) {}

	OrderRequest() = default;
};
