#ifndef ZIA_ROUTING_H_
#define ZIA_ROUTING_H_

#include "../event.hpp"
#include "message.hpp"

namespace zia::api::http
{

struct Route {
    std::string path;
    std::unordered_map<std::string, std::string> query_parameters;
};

class RouteRequestEvent
{
    virtual const Route &getRoute() = 0;
    virtual const HTTPMessage &getMessage() = 0;

    virtual void routeNext() = 0;
};
} // namespace zia::api::http
#endif // ZIA_ROUTING_H_
