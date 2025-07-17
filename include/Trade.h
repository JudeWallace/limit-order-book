#pragma once

#include <string>

#include "Using.h"

struct Trade {
	OrderId takerOrder;
	OrderId restingOrder;
	Price matchedPrice;
	Quantity quantity;
	std::string timestamp;
};