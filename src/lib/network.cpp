#include "lib/network.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <regex>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace dev
{
namespace network
{

class HttpClientImpl : public HttpClient
{
public:
    HttpClientImpl() = default;

    void setTimeout(uint32_t timeoutMs) override { m_timeoutMs = timeoutMs; }

    void get(const std::string& url, CompleteHandler handler) override
    {
        doRequest(http::verb::get, url, std::string(), std::move(handler));
    }

    void post(const std::string& url, const std::string& body, CompleteHandler handler) override
    {
        doRequest(http::verb::post, url, body, std::move(handler));
    }

private:
    struct ParsedUrl
    {
        std::string host;
        std::string port;
        std::string target;
    };

    static ParsedUrl parseUrl(const std::string& url)
    {
        ParsedUrl p;
        std::string u = url;
        const std::string scheme = "http://";
        if (u.find(scheme) == 0)
            u = u.substr(scheme.size());

        auto slash = u.find('/');
        if (slash == std::string::npos)
        {
            p.host = u;
            p.target = "/";
        }
        else
        {
            p.host = u.substr(0, slash);
            p.target = u.substr(slash);
        }

        auto colon = p.host.find(':');
        if (colon == std::string::npos)
        {
            p.port = "80";
        }
        else
        {
            p.port = p.host.substr(colon + 1);
            p.host = p.host.substr(0, colon);
        }
        return p;
    }

    void doRequest(http::verb method, const std::string& url, const std::string& body,
        CompleteHandler handler)
    {
        auto parsed = parseUrl(url);
        int version = 11;

        try
        {
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);

            auto const results = resolver.resolve(parsed.host, parsed.port);
            stream.connect(results);

            http::request<http::string_body> req{method, parsed.target, version};
            req.set(http::field::host, parsed.host);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            if (method == http::verb::post)
            {
                req.set(http::field::content_type, "application/x-www-form-urlencoded");
                req.body() = body;
                req.prepare_payload();
            }

            http::write(stream, req);

            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            if (handler)
                handler(static_cast<int>(res.result_int()), res.body());
        }
        catch (const std::exception& e)
        {
            std::cerr << "HttpClient error: " << e.what() << "\n";
            if (handler)
                handler(0, std::string{});
        }
    }

    uint32_t m_timeoutMs = 5000;
};

HttpClient::Ptr createHttpClient()
{
    return std::make_shared<HttpClientImpl>();
}

}  // namespace network
}  // namespace dev