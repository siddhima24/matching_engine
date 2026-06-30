// order_book.cpp
#include "order_book.hpp"

std::vector<Trade> OrderBook::addOrder(Order order) {
    std::vector<Trade> trades;
    auto& opposite = (order.side == Side::Buy) ? asks_ : bids_;

    while (order.quantity > 0 && !opposite.empty()) {
        auto best_it = (order.side == Side::Buy) ? opposite.begin() : std::prev(opposite.end());
        double best_price = best_it->first;

        bool crosses = (order.side == Side::Buy) ? (order.price >= best_price)
                                                   : (order.price <= best_price);
        if (!crosses) break;

        auto& level = best_it->second;
        Order& resting = level.front();
        int fill_qty = std::min(order.quantity, resting.quantity);

        uint64_t buy_id  = (order.side == Side::Buy) ? order.id : resting.id;
        uint64_t sell_id = (order.side == Side::Buy) ? resting.id : order.id;
        trades.push_back({buy_id, sell_id, best_price, fill_qty});

        order.quantity -= fill_qty;
        resting.quantity -= fill_qty;

        if (resting.quantity == 0) {
            index_.erase(resting.id);
            level.pop_front();
            if (level.empty()) opposite.erase(best_it);
        }
    }

    if (order.quantity > 0) {
        auto& same_side = (order.side == Side::Buy) ? bids_ : asks_;
        same_side[order.price].push_back(order);
        index_[order.id] = {order.side, order.price};
    }

    return trades;
}

bool OrderBook::cancelOrder(uint64_t order_id) {
    auto it = index_.find(order_id);
    if (it == index_.end()) return false;

    auto& book = (it->second.side == Side::Buy) ? bids_ : asks_;
    auto level_it = book.find(it->second.price);
    if (level_it == book.end()) return false;

    auto& dq = level_it->second;
    for (auto o_it = dq.begin(); o_it != dq.end(); ++o_it) {
        if (o_it->id == order_id) { dq.erase(o_it); break; }
    }
    if (dq.empty()) book.erase(level_it);

    index_.erase(it);
    return true;
}