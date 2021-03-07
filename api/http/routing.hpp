#ifndef ZIA_HTTP_ROUTING_API_H_
#define ZIA_HTTP_ROUTING_API_H_

#include "../event.hpp"
#include "request.hpp"

namespace zia::api::http
{

struct Route {
    std::string path;
    std::unordered_map<std::string, std::string> query_parameters;
};

// Helper struct to create an EventDescriptor for a router event
struct RouteEventDescriptor {
    RouteEventDescriptor(const std::string_view &handler)
        : name("RouteRequestEvent-" + std::string(handler)), descriptor{name}
    {
    }

    std::string name;
    EventDescriptor descriptor;
};

// Event sent by the router to the different route handlers
class RouteRequestEvent : public IEvent
{
public:
    RouteRequestEvent(const std::string_view &handler): m_descriptor(handler)
    {
    }

    virtual const Route &getRoute() = 0;
    virtual const HTTPRequest &getRequest() = 0;
    virtual std::unique_ptr<IEvent>
    createResponse(const api::http::HTTPResponse &response) = 0;

    const EventDescriptor &getDescriptor() const final
    {
        return m_descriptor.descriptor;
    }

private:
    RouteEventDescriptor m_descriptor;
};
}    // namespace zia::api::http
#endif    // ZIA_HTTP_ROUTING_API_H_
