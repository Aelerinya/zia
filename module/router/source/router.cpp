#include <iostream>
#include <yaml-cpp/yaml.h>

#include "router.hpp"
#include "utils.hpp"

using namespace zia::router;

RouteRequestEvent::RouteRequestEvent(const std::string_view &handler,
                                     const api::http::Route &route,
                                     std::unique_ptr<api::http::NewHTTPRequest> request)
    : api::http::RouteRequestEvent(handler),
      m_route(route),
      m_request_event(std::move(request))
{
}

zia::api::http::Route &RouteRequestEvent::getRoute()
{
    return m_route;
}
const zia::api::http::HTTPRequest &RouteRequestEvent::getRequest()
{
    return m_request_event->getRequest();
}

std::unique_ptr<zia::api::IEvent>
RouteRequestEvent::createResponse(const api::http::HTTPResponse &response)
{
    return m_request_event->createResponse(response);
}

const std::string &Router::getName() const
{
    static const std::string name = "router";
    return name;
}

const std::vector<Router::RouteConfig> &Router::getRoutes() const
{
    return m_config;
}

std::optional<Router::RoutingResult>
Router::routeRequest(const std::string_view &route) const
{
    for (auto config: m_config) {
        if (route.starts_with(config.prefix)) {
            Router::RoutingResult result{
                config.handler, {std::string(route.substr(config.prefix.size())), {}}};
            return result;
        }
    }
    return std::nullopt;
}

void Router::configureModule(const YAML::Node &config)
{
    m_config.clear();

    if (!config.IsMap())
        throw InvalidConfiguration("Router", config, "config is not a map");
    for (auto value: config) {
        auto route = value.first.as<std::string>();
        auto handler = value.second["handler"].as<std::string>();
        std::clog << "Loaded route: " << route << " → " << handler << std::endl;
        m_config.emplace_back(route, handler);
    }
}
