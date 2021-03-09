#include "api/http/routing.hpp"
#include "api/initializer.hpp"
#include "api/mediator.hpp"
#include "static.hpp"
#include "utils.hpp"

namespace
{
const zia::api::http::RouteEventDescriptor static_descriptor("static");
}

extern "C" std::unique_ptr<zia::api::IModule>
load_module(zia::api::IZiaInitializer &initializer)
{
    auto static_module = std::make_unique<zia::Static>();

    initializer.registerConsumer(
        static_descriptor.descriptor,
        [&static_module = *static_module](zia::api::IZiaMediator &mediator,
                                          std::unique_ptr<zia::api::IEvent> e) {
            auto event = zia::dynamic_unique_ptr_cast<zia::api::http::RouteRequestEvent>(
                std::move(e));
            auto response =
                static_module.handleRequest(event->getRoute(), event->getRequest());
            mediator.emit(event->createResponse(response));
        });

    return static_module;
}
