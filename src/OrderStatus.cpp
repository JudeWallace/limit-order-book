#include "OrderStatus.h"

std::string to_string(OrderStatus status) {
	switch (status) {
	case OrderStatus::Filled:
		return "Filled";
	case OrderStatus::PartiallyFilled:
		return "PartiallyFilled";
	case OrderStatus::NoLiquidity:
		return "NoLiquidity";
	case OrderStatus::Rejected:
		return "Rejected";
	case OrderStatus::Cancelled:
		return "Cancelled";
	case OrderStatus::PlacedOrder:
		return "PlacedOrder";
	case OrderStatus::SelfTradePrevention:
		return "SelfTradePrevention";
	default:
		return "Unknown";
	}
};

std::ostream &operator<<(std::ostream &os, OrderStatus status) {
	return os << to_string(status);
};
