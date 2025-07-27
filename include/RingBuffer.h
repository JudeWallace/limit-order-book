#pragma once

#include "OrderRequest.h"
#include <thread>
#include <atomic>

constexpr size_t SIZE = 1024;

class RingBuffer {
    private:
        std::array<OrderRequest, SIZE> buffer_;
        std::array<std::atomic<size_t>, SIZE> sequences_;
        std::atomic<size_t> writeIdx_{0};
        size_t readIdx_{0};
        const size_t mask_ = SIZE - 1;

    public:
        RingBuffer();

        bool enqueue();
        bool dequeue();
};
