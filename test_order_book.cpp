#include <iostream>
#include <cassert>
#include "order_book.hpp"

void test_full_match() {
    OrderBook book;
    book.addOrder({1, Side::Sell, 100.0, 10, {}});
    auto trades = book.addOrder({2, Side::Buy, 100.0, 10, {}});

    assert(trades.size() == 1);
    assert(trades[0].quantity == 10);
    std::cout << "test_full_match PASSED\n";
}

void test_price_time_priority() {
    OrderBook book;
    book.addOrder({1, Side::Sell, 100.0, 5, {}});  // arrives first
    book.addOrder({2, Side::Sell, 100.0, 5, {}});  // arrives second
    auto trades = book.addOrder({3, Side::Buy, 100.0, 5, {}});

    assert(trades[0].sell_order_id == 1);  // the FIRST one should match, not the second
    std::cout << "test_price_time_priority PASSED\n";
}

void test_partial_match() {
    OrderBook book;
    book.addOrder({1, Side::Sell, 100.0, 4, {}});
    auto trades = book.addOrder({2, Side::Buy, 100.0, 10, {}});

    assert(trades[0].quantity == 4);   // only 4 could be filled
    std::cout << "test_partial_match PASSED\n";
}

void test_no_cross() {
    OrderBook book;
    book.addOrder({1, Side::Sell, 105.0, 10, {}});
    auto trades = book.addOrder({2, Side::Buy, 100.0, 10, {}});

    assert(trades.size() == 0);  // buyer won't pay 105, so nothing should match
    std::cout << "test_no_cross PASSED\n";
}

void test_cancel() {
    OrderBook book;
    book.addOrder({1, Side::Sell, 100.0, 10, {}});
    bool cancelled = book.cancelOrder(1);
    auto trades = book.addOrder({2, Side::Buy, 100.0, 10, {}});

    assert(cancelled == true);
    assert(trades.size() == 0);  // the cancelled order shouldn't be there to match
    std::cout << "test_cancel PASSED\n";
}

int main() {
    test_full_match();
    test_price_time_priority();
    test_partial_match();
    test_no_cross();
    test_cancel();
    std::cout << "\nALL TESTS PASSED\n";
}