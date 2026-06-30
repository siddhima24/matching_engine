// api.cpp
#include <httplib.h>
#include "order_book.hpp"
#include <sstream>

int main() {
    OrderBook book;
    httplib::Server svr;

    svr.Get("/book", [&](const httplib::Request&, httplib::Response& res) {
        // serialize top-of-book bids/asks to JSON, return res.set_content(...)
    });

    svr.Post("/order", [&](const httplib::Request& req, httplib::Response& res) {
        // parse JSON body into an Order, call book.addOrder(order), return trades as JSON
    });

    svr.listen("0.0.0.0", 8080);
}