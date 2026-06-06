#pragma once

#include <string>
#include <functional>
#include <memory>

namespace dev
{
    namespace network
    {

        class HttpClient
        {
        public:
            using Ptr = std::shared_ptr<HttpClient>;
            using CompleteHandler = std::function<void(int status, const std::string& body)>;

            virtual ~HttpClient() = default;

            virtual void setTimeout(uint32_t timeoutMs) = 0;

            virtual void get(const std::string& url, CompleteHandler handler) = 0;
            virtual void post(const std::string& url, const std::string& body, CompleteHandler handler) = 0;
        };

        HttpClient::Ptr createHttpClient();

    }  // namespace network
}  // namespace dev