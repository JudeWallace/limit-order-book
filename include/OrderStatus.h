#pragma once

#include <iostream>
#include <string>

enum class OrderStatus { Filled, PartiallyFilled, NoLiquidity, Rejected, Cancelled, PlacedOrder, SelfTradePrevention };

std::string to_string(OrderStatus os);
std::ostream &operator<<(std::ostream &os, OrderStatus status);
