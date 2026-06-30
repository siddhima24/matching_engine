// order.hpp
#pragma once
#include <cstdint>
#include <chrono>

enum class Side { Buy, Sell };

struct Order {
    uint64_t id;
    Side side;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;
};

struct Trade {
    uint64_t buy_order_id;
    uint64_t sell_order_id;
    double price;
    int quantity;
};