#pragma once

enum OrderStatus {
	Filled,
	PartiallyFilled,
	NoLiquidity,
	Rejected,
	Cancelled,
	PlacedOrder,
	SelfTradePrevention
};