set(THIS OrderBookTestSuite)
set(TEST_SOURCES OrderBookTests.cpp)

add_executable(${THIS} ${TEST_SOURCES})

target_link_libraries(${THIS}
    PRIVATE
    gtest_main
    limitOrderBookLib
)

include(GoogleTest)
gtest_discover_tests(${THIS})