#pragma once

enum class OrderStatus {
	Filled,
	PartiallyFilled,
	NoLiquidity,
	Rejected,
	Cancelled,
	PlacedOrder,
	SelfTradePrevention
};