#include <gtest/gtest.h>
#include "OrderBook.h" 

TEST(OrderBookTest, FirstTest) {
    OrderBook ob;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
