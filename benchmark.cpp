// benchmark.cpp
#include <chrono>
#include <random>
#include <iostream>
#include "order_book.hpp"

int main() {
    OrderBook book;
    const int N = 1'000'000;
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> price_dist(95.0, 105.0);
    std::uniform_int_distribution<int> qty_dist(1, 100);
    std::bernoulli_distribution side_dist(0.5);

    std::vector<Order> orders;
    for (int i = 0; i < N; ++i) {
        orders.push_back({(uint64_t)i, side_dist(rng) ? Side::Buy : Side::Sell,
                           price_dist(rng), qty_dist(rng), {}});
    }

    auto start = std::chrono::steady_clock::now();
    for (auto& o : orders) book.addOrder(o);
    auto end = std::chrono::steady_clock::now();

    double secs = std::chrono::duration<double>(end - start).count();
    std::cout << N << " orders in " << secs << "s -> "
              << (N / secs) << " orders/sec\n";
}