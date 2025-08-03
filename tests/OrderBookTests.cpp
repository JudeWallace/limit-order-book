#include <gtest/gtest.h>
#include "OrderBook.h" 

TEST(OrderBookTest, MarketOrderNoLiquidity) {
    OrderBook ob;

    ob.addOrder(1, 1, "TEST1", OrderType::Market, Side::BUY, 100);
    Bids bidsRef = ob.getBids();
    bool isEmpty = bidsRef.empty();
    
    ASSERT_TRUE(isEmpty);
}

TEST(OrderBookTest, LimitOrderNoLiquidty) {
    OrderBook ob;

    ob.addOrder(1, 1, "TEST1", OrderType::Limit, Side::BUY, 80, 100);
    Bids bidsRef = ob.getBids();
    bool isEmpty = bidsRef.empty();
    auto it = bidsRef.begin();
    
    ASSERT_FALSE(isEmpty);
    ASSERT_TRUE(it->first == 80);
}

TEST(OrderBookTest, LimitMarketMatch) {
    OrderBook ob;
    Bids bidsRef = ob.getBids();
    Asks asksRef = ob.getAsks();

    ob.addOrder(1, 1, "TEST1", OrderType::Limit, Side::BUY, 80, 100);
    ob.addOrder(2, 2, "TEST2", OrderType::Market, Side::SELL, 100);

    bool BidsIsEmpty = bidsRef.empty();
    bool AsksIsEmpty = asksRef.empty();
    
    ASSERT_TRUE(BidsIsEmpty);
    ASSERT_TRUE(AsksIsEmpty);
}

TEST(OrderBookTest, SelfTradePreventionRTO) {
    OrderBook ob;

    ob.addOrder(1, 1, "TEST1", OrderType::Limit, Side::BUY, 80, 100);
    ob.addOrder(2, 1, "TEST2", OrderType::Limit, Side::SELL, 80, 100);

    Bids bidsRef = ob.getBids();
    Asks asksRef = ob.getAsks();


    bool BidsIsEmpty = bidsRef.empty();
    
    ASSERT_FALSE(BidsIsEmpty);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
