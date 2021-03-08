#include "ModuleHub.hpp"
#include "module/test/test_module.hpp"
#include <boost/test/unit_test.hpp>

using namespace zia;

BOOST_AUTO_TEST_CASE(test_ModuleHub)
{
    ModuleHub hub;

    hub.loadModule("lib/testmodule");
    BOOST_TEST(hub.getModule(test::Module::module_name).has_value());
    hub.unloadModule(test::Module::module_name);
    BOOST_TEST(!hub.getModule(test::Module::module_name).has_value());
    hub.loadModule("lib/testmodule");
    BOOST_TEST(hub.getModule(test::Module::module_name).has_value());

    BOOST_TEST(hub.getEventConsumer(api::event_descriptor<test::Event>).has_value());
    BOOST_TEST(hub.getEventListeners(api::event_descriptor<test::Event>).size() == 1);
    hub.unloadModule(test::Module::module_name);
    BOOST_TEST(!hub.getEventConsumer(api::event_descriptor<test::Event>).has_value());
    BOOST_TEST(hub.getEventListeners(api::event_descriptor<test::Event>).size() == 0);
    hub.loadModule("lib/testmodule");
}
