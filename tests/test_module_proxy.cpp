#include "ModuleProxy.hpp"
#include "TestInitializer.hpp"
#include "TestMediator.hpp"
#include "module/test/test_module.hpp"
#include <boost/test/unit_test.hpp>
#include <yaml-cpp/yaml.h>

using namespace zia;

class TestEvent : public zia::api::IEvent
{
    const api::EventDescriptor &getDescriptor() const override;
};

template <>
constexpr zia::api::EventDescriptor zia::api::event_descriptor<TestEvent> = {"TestEvent"};

const api::EventDescriptor &TestEvent::getDescriptor() const
{
    return zia::api::event_descriptor<TestEvent>;
}

BOOST_AUTO_TEST_CASE(test_ModuleProxy)
{
    YAML::Load("key: value");
    test::Initializer initializer;
    test::Mediator mediator;
    zia::ModuleProxy proxy("lib/libtestmodule.so", initializer);

    std::clog << initializer.consumers.at(0).first.get().name << std::endl;
    BOOST_TEST(proxy.getName() == test::Module::module_name);
    std::clog << "getting consumer" << std::endl;
    BOOST_TEST(initializer.consumers.at(0).first.get().name ==
               api::event_descriptor<test::Event>.name);
    initializer.consumers.at(0).second(mediator, TestEvent());
    BOOST_TEST(mediator.events.at(0)->getDescriptor().name ==
               api::event_descriptor<test::Event>.name);
    initializer.listeners.at(0).second(mediator, TestEvent());
    std::clog << "getting listener" << std::endl;
    BOOST_TEST(initializer.listeners.at(0).first.get().name ==
               api::event_descriptor<test::Event>.name);
    std::clog << "end" << std::endl;
    initializer.consumers.clear();
    initializer.listeners.clear();
    mediator.events.clear();
}
