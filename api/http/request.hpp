#ifndef ZIA_HTTP_REQUEST_API_H_
#define ZIA_HTTP_REQUEST_API_H_

#include "../event.hpp"
#include <string>
#include <unordered_map>

namespace zia::api::http
{
enum class HTTPMethod
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

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
};
} // namespace zia::api::http
#endif // ZIA_HTTP_REQUEST_API_H_
