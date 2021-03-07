#ifndef ZIA_ROUTER_H_
#define ZIA_ROUTER_H_

#include <optional>

#include "api/http/request.hpp"
#include "api/http/routing.hpp"
#include "api/module.hpp"
#include "utils.hpp"

// FIXME: Use API request type only
#include "module/server/include/request.hpp"

namespace zia::router
{
class RouteRequestEvent : public api::http::RouteRequestEvent
{
public:
    RouteRequestEvent(const std::string_view &handler, const api::http::Route &route,
                      std::unique_ptr<api::http::NewHTTPRequest> request);

    api::http::Route &getRoute() override;
    const api::http::HTTPRequest &getRequest() override;

    std::unique_ptr<IEvent>
    createResponse(const api::http::HTTPResponse &response) override;

private:
    api::http::Route m_route;
    std::unique_ptr<api::http::NewHTTPRequest> m_request_event;
};

class Router : public api::IModule
{
public:
    const std::string &getName() const override;
    void configureModule(const YAML::Node &) override;

    struct RouteConfig {
        std::string prefix;
        std::string handler;
    };
    const std::vector<RouteConfig> &getRoutes() const;

    struct RoutingResult {
        std::string handler;
        api::http::Route route;
    };
    std::optional<RoutingResult> routeRequest(const std::string_view &route) const;

private:
    std::vector<RouteConfig> m_config;
};
}    // namespace zia::router

#endif    // ZIA_ROUTER_H_
