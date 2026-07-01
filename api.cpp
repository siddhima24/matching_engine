#define _WIN32_WINNT 0x0A00
#include <windows.h>



#include "httplib.h"
#include "order_book.hpp"
#include <sstream>
#include <iostream>

// Turns a snapshot of the book into a simple JSON string by hand
// (no extra libraries needed — just building text)
std::string bookToJson(const OrderBook& book) {
    auto snap = book.getSnapshot(10); // we'll add this function below
    std::ostringstream out;
    out << "{\"bids\":[";
    for (size_t i = 0; i < snap.bids.size(); i++) {
        out << "{\"price\":" << snap.bids[i].first << ",\"qty\":" << snap.bids[i].second << "}";
        if (i + 1 < snap.bids.size()) out << ",";
    }
    out << "],\"asks\":[";
    for (size_t i = 0; i < snap.asks.size(); i++) {
        out << "{\"price\":" << snap.asks[i].first << ",\"qty\":" << snap.asks[i].second << "}";
        if (i + 1 < snap.asks.size()) out << ",";
    }
    out << "]}";
    return out.str();
}

// Very simple manual parsing — looks for "side", "price", "quantity" in the
// incoming text and pulls out the values after them. Not a real JSON parser,
// just enough for this one specific use.
double extractNumber(const std::string& body, const std::string& key) {
    size_t pos = body.find("\"" + key + "\"");
    pos = body.find(":", pos) + 1;
    return std::stod(body.substr(pos));
}

std::string extractString(const std::string& body, const std::string& key) {
    size_t pos = body.find("\"" + key + "\"");
    pos = body.find(":", pos) + 1;
    pos = body.find("\"", pos) + 1;
    size_t end = body.find("\"", pos);
    return body.substr(pos, end - pos);
}

int main() {
    OrderBook book;
    httplib::Server svr;
    uint64_t next_id = 1;

    svr.Get("/book", [&](const httplib::Request&, httplib::Response& res) {
        res.set_content(bookToJson(book), "application/json");
    });

    svr.Post("/order", [&](const httplib::Request& req, httplib::Response& res) {
    std::cerr << "RAW BODY RECEIVED: [" << req.body << "]\n"; 
    try {
        std::string side_str = extractString(req.body, "side");
        double price = extractNumber(req.body, "price");
        int qty = (int)extractNumber(req.body, "quantity");

        Side side = (side_str == "buy") ? Side::Buy : Side::Sell;
        Order order{next_id++, side, price, qty, {}};
        auto trades = book.addOrder(order);

        std::ostringstream out;
        out << "{\"trades_executed\":" << trades.size() << ",\"book\":" << bookToJson(book) << "}";
        res.set_content(out.str(), "application/json");
    } catch (const std::exception& e) {
        std::cerr << "ERROR handling /order: " << e.what() << "\n";
        res.status = 500;
        res.set_content(std::string("{\"error\":\"") + e.what() + "\"}", "application/json");
    }
});

    std::cout << "Server running at http://localhost:8080\n";
    std::cout << "Try opening http://localhost:8080/book in your browser\n";
    svr.listen("0.0.0.0", 8080);
}