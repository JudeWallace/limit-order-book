cmake_minimum_required(VERSION 3.15)
project(LimitOrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Boost setup ===
find_package(Boost REQUIRED COMPONENTS system thread)

# === Include directories ===
include_directories(
    include
    external
    ${Boost_INCLUDE_DIRS}
)

# === Source files ===
set(SOURCES
    src/Main.cpp
    src/Server.cpp
    src/OrderBook.cpp
    src/OrderNode.cpp
    src/PriceLevel.cpp
    src/OrderStatus.cpp
    src/RingBuffer.cpp
)

add_executable(main ${SOURCES})

# Link Boost libraries
target_link_libraries(main PRIVATE Boost::system Boost::thread)

# Define preprocessor macros for Crow using Boost.Asio
target_compile_definitions(main PRIVATE CROW_USE_BOOST CROW_USE_BOOST_ASIO)

# Also add pthread (required on Linux for threading)
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)
