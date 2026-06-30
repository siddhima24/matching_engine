// order_book.hpp
#pragma once
#include <map>
#include <deque>
#include <unordered_map>
#include <vector>
#include "order.hpp"

class OrderBook {
public:
    std::vector<Trade> addOrder(Order order);
    bool cancelOrder(uint64_t order_id);

private:
    struct Location { Side side; double price; };

    std::map<double, std::deque<Order>> bids_; // descending priority = highest price
    std::map<double, std::deque<Order>> asks_; // ascending priority = lowest price
    std::unordered_map<uint64_t, Location> index_;
};