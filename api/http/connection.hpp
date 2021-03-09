#ifndef ZIA_HTTP_CONNECTION_API_H_
#define ZIA_HTTP_CONNECTION_API_H_

#include "api/mediator.hpp"
#include <boost/asio.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace zia::api::http
{

class NewHTTPConnectionEvent : public IEvent
{
public:
    const EventDescriptor &getDescriptor() const final;
};

}    // namespace zia::api::http

namespace zia::api
{
template <>
constexpr EventDescriptor event_descriptor<http::NewHTTPConnectionEvent> = {
    "NewHTTPConnectionEvent"};

inline const EventDescriptor &http::NewHTTPConnectionEvent ::getDescriptor() const
{
    return event_descriptor<http::NewHTTPConnectionEvent>;
}

}    // namespace zia::api
#endif
