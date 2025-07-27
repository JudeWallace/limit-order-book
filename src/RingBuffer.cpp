#include "RingBuffer.h"

RingBuffer::RingBuffer() {
    for (size_t i = 0; i < SIZE; ++i) {
        sequences_[i].store(i, std::memory_order_relaxed);
    }
}

bool RingBuffer::enqueue() {
    size_t pos = writeIdx_.load(std::memory_order_relaxed);

    while (true) {
        size_t index = pos & mask_;
        size_t seq = sequences_[index].load(std::memory_order_acquire);
        intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos);

        if (diff == 0) {
            if (writeIdx_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed)) {
                buffer_[index] = OrderRequest();
                sequences_[index].store(pos + 1, std::memory_order_release);
                return true;
            }
        } else if (diff < 0) {
            // Buffer full
            return false;
        } else {
            pos = writeIdx_.load(std::memory_order_relaxed);
        }
    }
}

bool RingBuffer::dequeue() {
    size_t pos = readIdx_;
    size_t index = pos & mask_;
    size_t seq = sequences_[index].load(std::memory_order_acquire);
    intptr_t diff = static_cast<intptr_t>(seq) - static_cast<intptr_t>(pos + 1);

    if (diff == 0) {
        auto data_out = buffer_[index];
        sequences_[index].store(pos + SIZE, std::memory_order_release);
        ++readIdx_;
        return true;
    }
    return false;
}