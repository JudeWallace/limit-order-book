cmake_minimum_required(VERSION 3.22)

set(THIS LimitOrderBook)
set(THIS_LIB limitOrderBookLib)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# === Boost setup ===
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Threads REQUIRED)

# === Include directories ===
include_directories(
    include
    external
    ${Boost_INCLUDE_DIRS}
)

# === Core library ===
add_library(${THIS_LIB}
    src/OrderBook.cpp
    src/OrderNode.cpp
    src/PriceLevel.cpp
    src/OrderStatus.cpp
    src/RingBuffer.cpp
    src/Server.cpp
)

target_include_directories(${THIS_LIB} PUBLIC
    ${PROJECT_SCOURCE_DIR}/include
    ${PROJECT_SCOURCE_DIR}/external
    ${Boost_INCLUDE_DIRS}
)

target_compile_definitions(${THIS_LIB} PRIVATE CROW_USE_BOOST CROW_USE_BOOST_ASIO)

# === Main executable ===
add_executable(main src/Main.cpp)

target_link_libraries(main
    PRIVATE
    ${THIS_LIB}
    Boost::system
    Boost::thread
    Threads::Threads
)

target_compile_definitions(main PRIVATE CROW_USE_BOOST CROW_USE_BOOST_ASIO)

# === Add GoogleTest ===
add_subdirectory(external/googletest)

# === Add tests ===
add_subdirectory(tests)

