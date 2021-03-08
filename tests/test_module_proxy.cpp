#include <boost/test/unit_test.hpp>
#include <yaml-cpp/yaml.h>

#include "ModuleProxy.hpp"
#include "TestInitializer.hpp"
#include "TestMediator.hpp"
#include "api/internal/onstart.hpp"
#include "module/test/test_module.hpp"
#include "TestDisplay.hpp"

using namespace zia;

BOOST_AUTO_TEST_CASE(test_ModuleProxy)
{
    YAML::Load("key: value");
    test::Initializer initializer;
    test::Mediator mediator;
    zia::ModuleProxy proxy("lib/testmodule", initializer);

    BOOST_TEST(proxy.getName() == test::Module::module_name);

    std::clog << "getting start consumer" << std::endl;
    BOOST_TEST(initializer.consumers.at(0).first.get() ==
               api::event_descriptor<zia::api::OnStartEvent>);
    initializer.consumers.at(0).second(mediator,
                                       std::make_unique<zia::api::OnStartEvent>());
    BOOST_TEST(mediator.events.at(0)->getDescriptor() ==
               api::event_descriptor<test::Event>);

    // Extracting event from the mediator
    std::unique_ptr<zia::api::IEvent> event;
    mediator.events.at(0).swap(event);
    mediator.events.clear();

    std::clog << "getting test listener" << std::endl;
    initializer.listeners.at(0).second(mediator, *event.get());
    BOOST_TEST(initializer.listeners.at(0).first.get() ==
               api::event_descriptor<test::Event>);

    std::clog << "getting test consumer" << std::endl;

    std::clog << "end" << std::endl;
    initializer.consumers.clear();
    initializer.listeners.clear();
    mediator.events.clear();
}
