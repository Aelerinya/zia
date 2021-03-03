#ifndef ZIA_HTTP_RESPONSE_API_H_
#define ZIA_HTTP_RESPONSE_API_H_

#include "../event.hpp"
#include <string>

namespace zia::api::http
{
struct HTTPResponse {
    unsigned short status_code;
    std::string status_message;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class NewHTTPResponse : public IEvent
{
    virtual const HTTPResponse &getResponse() const = 0;
    const EventDescriptor &getDescriptor() const final;
};
}    // namespace zia::api::http

namespace zia::api
{
template <>
constexpr EventDescriptor event_descriptor<http::NewHTTPResponse> = {"NewHTTPResponse"};

inline const EventDescriptor &http::NewHTTPResponse ::getDescriptor() const
{
    return event_descriptor<http::NewHTTPResponse>;
}

}    // namespace zia::api
#endif    // ZIA_HTTP_RESPONSE_API_H_
