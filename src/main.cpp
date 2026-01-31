#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

namespace bp = boost::process;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void daemon_task() {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    //主循环
    while(true) {
        http_server_task();
    }
}

int main(int argc, char *argv[]) {
    if(argc > 1 && std::string(argv[1]) == "--daemon") {
        daemon_task();
        return 0;
    }
    return 0;
}


bool token_validator(const http::request<http::string_body>& req) {
    static const std::unordered_map<std::string, std::string> valid_tokens = {
        {"admin_token", "admin"},
        {"user_token", "user"}
    };

    auto auth_header = req.find(http::field::authorization);
    if (auth_header != req.end()) {
        std::string auth(auth_header->value());
        if (auth.find("Bearer ") == 0) {
            std::string token = auth.substr(7);
            return valid_tokens.find(token) != valid_tokens.end();
        }
    }
    
    std::string target(req.target());
    size_t pos = target.find("?token=");
    if (pos != std::string::npos) {
        std::string token = target.substr(pos + 7);
        return valid_tokens.find(token) != valid_tokens.end();
    }
    
    return false;
}

void http_server_task() {
    try {
        net::io_context ioc(1);
        tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});
        
        std::cout << "HTTP Server listening on port 8080..." << std::endl;
        
        while (true) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            try {
                beast::flat_buffer buffer;
                http::request<http::string_body> req;
                http::read(socket, buffer, req);
                http::response<http::string_body> res{http::status::ok, req.version()};
                res.set(http::field::server, "Simple Server");
                res.set(http::field::content_type, "application/json");

                if (!token_validator(req)) {
                    res.result(http::status::unauthorized);
                    res.body() = R"({"error": "Unauthorized"})";
                } else {
                    if (req.method() == http::verb::get) {
                        res.body() = R"({"message": "Authenticated GET request"})";
                    } else if (req.method() == http::verb::post) {
                        res.body() = R"({"message": "Authenticated POST request"})";
                    } else {
                        res.result(http::status::method_not_allowed);
                        res.body() = R"({"error": "Method not allowed"})";
                    }
                }
                
                res.prepare_payload();
                http::write(socket, res);
                beast::error_code ec;
                socket.shutdown(tcp::socket::shutdown_send, ec);
                
            } catch (const std::exception& e) {
                std::cerr << "Request error: " << e.what() << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}
