#include "OrderBook.h"
#include "Server.h"

int main() {
	OrderBook ob;

	ob.addOrder(3, "LMT587", OrderType::Limit, Side::SELL, 80.0, 100);
	ob.addOrder(1, "LMT587", OrderType::Limit, Side::SELL, 90.0, 100);
	ob.addOrder(2, "LMT587", OrderType::FillOrKill, Side::BUY, 80.0, 80);

	std::cout << "=== Price Level ====" << std::endl;
	ob.printLevels();
	std::cout << "=======" << std::endl;

	Server server;
	server.run();

	/*
	ob.addOrder(2, "GTE123", OrderType::Limit, Side::BUY, 99.0, 40);
	ob.addOrder(2, "BME198", OrderType::Market, Side::SELL, 20);

	std::cout << "=== Price Level ====" << std::endl;
	ob.printLevels();
	std::cout << "=======" << std::endl;

	ob.addOrder(6, "BME198", OrderType::Limit, Side::BUY, 80.0, 100);

	std::cout << "=== Price Level ====" << std::endl;
	ob.printLevels();
	std::cout << "=======" << std::endl;

	ob.addOrder(5, "BME198", OrderType::Limit, Side::SELL, 80.0, 1000);
	ob.addOrder(4, "BME198", OrderType::Limit, Side::SELL, 80.0, 50);

	std::cout << "=== Price Level ====" << std::endl;
	ob.printLevels();
	std::cout << "=======" << std::endl;

	ob.cancelOrder(7);

	std::cout << "=== Price Level ====" << std::endl;
	ob.printLevels();
	std::cout << "=======" << std::endl;
	*/

	return 0;
}