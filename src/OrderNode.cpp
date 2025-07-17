#include "OrderNode.h"

OrderNode::OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Price p, Quantity quantity)
    : clientId(clId), clOrderId(exteneralClOrderId), orderId(id), side(s), price(p), type(orderType), initialQuantity(quantity), remainingQuantity(quantity) {}

OrderNode::OrderNode(ClientId clId, const std::string &exteneralClOrderId, OrderId id, OrderType orderType, Side s, Quantity quantity)
    : clientId(clId), clOrderId(exteneralClOrderId), orderId(id), side(s), price(0), type(orderType), initialQuantity(quantity), remainingQuantity(quantity) {}