module;

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unordered_map>
#include <iostream>

export module http_server;

namespace HttpServer {
    export void http_server_task();
    bool token_validator(const boost::beast::http::request<boost::beast::http::string_body> &req);
}