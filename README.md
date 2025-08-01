# Central limit order book

# Summary
This repository is my version of a high-performance limit order book implemented in C++. It is still a work in progress with the aim of pairing it up with a client frontend


# System architecture
add diagram here

## Server communications
This project currently uses Crow with Boost.Asio as a proof-of-concept. Eventually, the backend will be fully migrated to Boost libraries for improved performance and scalability.

You can find more information about Crow on their official website: https://crowcpp.org/master/

### Websocket
Due to the nature of this application, bi-directional communciation is essential. WebSockets are used for:
- Trade acknowledgments and updates
- Market data feeds
- Executing orders from the frontend (faster and more efficient than HTTP requests)

### REST
For less time-sensitive parts of the application, a simple REST API will be exposed. 

## Live Market data feed
TODO

## Redis and SQL 
TODO


# Key Concepts

## Order types
This is a LOB that can handle many different order types like a real exchange. The current implemntation works for:

- Market
- Limit
- Fill or kill
- Good till day

## LMAX Disruptor pattern
This is the inspiration for how orders are piped to the order book. The LMAX Disruptor pattern was originally developed by the LMAX Exchange to achieve extremely high throughput and low latency for financial trading systems. It is a lock-free, single-writer ring buffer that allows multiple consumer threads to process events in a time-priority and deterministic order.


For my implementation, I developed a multi-producer, single-consumer (MPSC) ring buffer model. This architecture allows multiple threads (or upstream components) to enqueue orders concurrently, while a single dedicated consumer thread processes them in strict time order — just like the LMAX Disruptor’s core intent. This single consumer thread also reduces the complexity of the datastructure and the requirement to handle batch processing of orders (Future improvements could change this).

<b>Key Design Elements:</b>
* Multiple Producers: Multiple threads can safely enquire orders without contention.
* Single Consumer: A single thread processing orders in the ring buffer, preserving the time-priority requirement.
* Lock-Free via Sequencing: Like LMAX, I avoided mutex's by using sequence numbers to manager buffer slots. Each producers atomically claims a slot, write its data and signals it's ready to be read via a published sequence.
    * Idea being a consumer can only consume if the buffer slot sequence number == consumer sequence value
* Wait strategy: If the buffer is full at the time of enqueue a spin-wait is in place so the thread yeilds and try's again

## Self trade prevention
Self-trade prevention (STP) is a regulatory control implemented by exchanges to prevent a market participant from unintentionally trading with themselves, which can create misleading signals about market demand, inflate volume, or distort price discovery. Practices that may be considered manipulative under market abuse regulations.

While most regulated exchanges enforce STP rules to maintain market integrity, traders are also expected to implement their own self-trade prevention mechanisms to ensure compliance with regulatory obligations.

In this implemation LOB the following STP measures have been implemented:
- Reject taking order (RTO)
- Reject resting order (RRO)
- Rejext both orders (RBO) 

By default all orders are set to RTO but clients can specifiy there preference in the frontend.

# Build steps

mkdir build && cd build && cmake .. && make