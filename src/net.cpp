#include "net.h"
#include "lib/network.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

void use_raw_boost_net() {
    // 1. Set up the network objects
    net::io_context ioc;
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // 2. Resolve the host and connect
    auto const host = "httpbin.org";
    auto const port = "80";
    auto const target = "/get";
    int version = 11; // HTTP/1.1

    auto const results = resolver.resolve(host, port);
    stream.connect(results);

    // 3. Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // 4. Send the HTTP request to the remote host
    http::write(stream, req);

    // 5. Declare a buffer to receive the response
    beast::flat_buffer buffer;

    // 6. Declare a container for the response
    http::response<http::string_body> res;

    // 7. Receive the HTTP response
    http::read(stream, buffer, res);

    // 8. Print the result
    std::cout << "Status: " << res.result_int() << " " << res.reason() << "\n";
    std::cout << "Body: " << res.body() << "\n";

    // 9. Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // disconnected means the socket was closed by the server, which is fine
    if(ec && ec != beast::errc::not_connected)
        throw beast::system_error{ec};
}

void use_network() {
    auto client = dev::network::createHttpClient();
    client->get("http://httpbin.org/get", [](int status, const std::string& body) {
        std::cout << "Status: " << status << "\n";
        std::cout << "Body: " << body << "\n";
    });
}
