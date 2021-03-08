#include <boost/test/unit_test.hpp>

#include "Executor.hpp"
#include "ModuleHub.hpp"
#include "TestDisplay.hpp"
#include "api/internal/onstart.hpp"
#include "module/test/test_module.hpp"

BOOST_AUTO_TEST_CASE(test_Executor)
{
    zia::ModuleHub hub;

    hub.loadModule("lib/testmodule");
    BOOST_TEST(hub.getModule("TestModule").has_value());

    zia::Executor executor(hub);

    std::clog << "Executing start event" << std::endl;
    executor.handleEvent(std::make_unique<zia::api::OnStartEvent>());
    BOOST_TEST(executor.getEventQueue().size() == 1);
    BOOST_TEST(executor.getEventQueue().front()->getDescriptor() ==
               zia::api::event_descriptor<zia::test::Event>);

    std::clog << "Stepping executor" << std::endl;
    executor.step();
    BOOST_TEST(executor.getEventQueue().size() == 0);
}
