#pragma once

#include <vector>

#include "OrderStatus.h"
#include "Trade.h"

struct MatchResult {
	OrderStatus status = OrderStatus::NoLiquidity;
	std::vector<Trade> executedTrades;
};