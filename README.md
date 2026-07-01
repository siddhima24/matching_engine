# Limit Order Book Matching Engine

A C++ implementation of a price-time priority matching engine — the core
logic behind how exchanges (NYSE, Binance, etc.) match buy and sell orders.

## How it works
Orders are stored in price levels using `std::map` (a sorted tree structure),
with a FIFO queue at each price level to preserve time priority. This gives
O(log n) insertion and cancellation, compared to O(n) cancellation with a
heap-based approach, which matters since cancel rates are typically much
higher than fill rates in real markets.

## Results
- Benchmarked at ~1.33 million orders/second (single-threaded, -O2, i5-1235U)
- 5 passing tests covering full match, partial match, price-time priority,
  no-cross scenarios, and cancellation

## How to run
\`\`\`bash
g++ -std=c++17 order_book.cpp benchmark.cpp -o benchmark.exe
./benchmark.exe

g++ -std=c++17 order_book.cpp test_order_book.cpp -o tests.exe
./tests.exe
\`\`\`

## Files
- `order.hpp` — core data types (Order, Trade)
- `order_book.hpp` / `order_book.cpp` — the matching engine
- `benchmark.cpp` — throughput test
- `test_order_book.cpp` — correctness tests
