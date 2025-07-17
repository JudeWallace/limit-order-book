#pragma once

enum class OrderType {
	Market,
	Limit,
	FillOrKill,
	ImmediateOrCancel,
	GoodTilCancelled,
	GoodTilTime
};