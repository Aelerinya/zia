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

class RouteRequestEvent
{
    virtual const Route &getRoute() = 0;
    virtual const HTTPRequest &getRequest() = 0;

    virtual void routeNext() = 0;
};
} // namespace zia::api::http
#endif // ZIA_HTTP_ROUTING_API_H_
