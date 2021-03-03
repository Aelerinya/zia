#ifndef ZIA_HTTP_REQUEST_API_H_
#define ZIA_HTTP_REQUEST_API_H_

#include "../event.hpp"
#include <string>
#include <unordered_map>

namespace zia::api::http
{
enum class HTTPMethod { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH };

struct HTTPRequest {
    HTTPMethod method;
    std::string route;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class NewHTTPRequest : public IEvent
{
public:
    virtual const HTTPRequest &getRequest() = 0;
    const EventDescriptor &getDescriptor() const final;
};
}    // namespace zia::api::http

namespace zia::api
{
template <>
constexpr EventDescriptor event_descriptor<http::NewHTTPRequest> = {"NewHTTPRequest"};

inline const EventDescriptor &http::NewHTTPRequest ::getDescriptor() const
{
    return event_descriptor<http::NewHTTPRequest>;
}

}    // namespace zia::api
#endif    // ZIA_HTTP_REQUEST_API_H_
