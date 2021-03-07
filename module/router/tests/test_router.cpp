#define BOOST_TEST_MODULE Router
#include <boost/test/included/unit_test.hpp>
#include <yaml-cpp/yaml.h>

#include "router.hpp"

using namespace zia::router;

BOOST_AUTO_TEST_CASE(test_router)
{
    auto config = YAML::Load("{/static: {handler: static}, /other: {handler: module}}");
    Router router;

    router.configureModule(config);
    BOOST_TEST(router.getRoutes().at(0).prefix == "/static");
    BOOST_TEST(router.getRoutes().at(0).handler == "static");
    BOOST_TEST(router.getRoutes().at(1).prefix == "/other");
    BOOST_TEST(router.getRoutes().at(1).handler == "module");
    auto result = router.routeRequest("/static/something");
    BOOST_TEST(result.has_value());
    BOOST_TEST(result.value().handler == "static");
    BOOST_TEST(result.value().route.path == "/something");
    BOOST_TEST(result.value().route.query_parameters.size() == 0);
    BOOST_TEST(!router.routeRequest("/nothing").has_value());
}

BOOST_AUTO_TEST_CASE(test_router_invalid)
{
    auto config = YAML::Load("[1, 2, 3]");
    Router router;
    BOOST_REQUIRE_THROW(router.configureModule(config), zia::InvalidConfiguration);
}
