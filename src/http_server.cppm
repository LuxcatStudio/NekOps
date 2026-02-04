module;

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <unordered_map>
#include <iostream>

module http_server;

namespace HttpServer {
    void http_server_task() {
        try {
            boost::asio::io_context ioc(1);
            boost::asio::ip::tcp::acceptor acceptor(ioc, {boost::asio::ip::tcp::v4(), 8080});
                
            std::cout << "HTTP Server listening on port 8080..." << std::endl;
                
            while (true) {
                boost::asio::ip::tcp::socket socket(ioc);
                acceptor.accept(socket);

                try {
                    boost::beast::flat_buffer buffer;
                    boost::beast::http::request<boost::beast::http::string_body> req;
                    boost::beast::http::read(socket, buffer, req);
                    boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, req.version()};
                    res.set(boost::beast::http::field::server, "Simple Server");
                    res.set(boost::beast::http::field::content_type, "application/json");

                    if (!token_validator(req)) {
                        res.result(boost::beast::http::status::unauthorized);
                        res.body() = R"({"error": "Unauthorized"})";
                    }
                    else {
                        if (req.method() == boost::beast::http::verb::get) {
                            res.body() = R"({"message": "Authenticated GET request"})";
                        }
                        else if (req.method() == boost::beast::http::verb::post) {
                            res.body() = R"({"message": "Authenticated POST request"})";
                        }
                        else {
                                res.result(boost::beast::http::status::method_not_allowed);
                                res.body() = R"({"error": "Method not allowed"})";
                        }
                    }
                        
                    res.prepare_payload();
                    boost::beast::http::write(socket, res);
                    boost::beast::error_code ec;
                    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
                        
                }
                catch (const std::exception &e) {
                    std::cerr << "Request error: " << e.what() << std::endl;
                }
            }
                
        }
        catch (const std::exception &e) {
            std::cerr << "Server error: " << e.what() << std::endl;
        }
    }
    bool token_validator(const boost::beast::http::request<boost::beast::http::string_body>& req) {
        static const std::unordered_map<std::string, std::string> valid_tokens = {
            {"admin_token", "admin"},
            {"user_token", "user"}
        };

        auto auth_header = req.find(boost::beast::http::field::authorization);
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
}