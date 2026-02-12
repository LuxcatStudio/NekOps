module;

#include <boost/beast.hpp>

export module http_server;

namespace HttpServer {
    export void http_server_task();
    bool token_validator(const boost::beast::http::request<boost::beast::http::string_body> &req);
}