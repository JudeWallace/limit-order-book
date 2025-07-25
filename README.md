# Central limit order book

# Summary

This repository is the backend written in C++ (work in progress)

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