cmake_minimum_required(VERSION 3.15)
project(LimitOrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include path
include_directories(include)

# Source files
set(SOURCES
    src/Main.cpp
    src/OrderBook.cpp
    src/OrderNode.cpp
    src/PriceLevel.cpp
)

add_executable(main ${SOURCES})
