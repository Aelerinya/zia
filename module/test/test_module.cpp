#include "test_module.hpp"
#include "api/internal/onstart.hpp"
#include "api/mediator.hpp"
#include <iostream>

using namespace zia;

test::Module::Module()
{
    std::clog << "Creating test module" << std::endl;
}

const std::string &test::Module::getName() const
{
    std::clog << "Getting test module name" << std::endl;
    return module_name;
}

void test::Module::configureModule([[maybe_unused]] const YAML::Node &config)
{
    std::clog << "Configuring test module" << std::endl;
}

ZIA_EXPORT_SYMBOL std::unique_ptr<api::IModule>
load_module([[maybe_unused]] api::IZiaInitializer &initializer)
{
    initializer.registerConsumer(
        api::event_descriptor<api::OnStartEvent>,
        [](api::IZiaMediator &mediator, std::unique_ptr<api::IEvent>) {
            std::clog << "Start event called" << std::endl;
            mediator.emit(std::make_unique<test::Event>());
        });
    initializer.registerListener(api::event_descriptor<test::Event>,
                                 [](const api::IZiaMediator &, const api::IEvent &) {
                                     std::clog << "Listener called" << std::endl;
                                 });
    initializer.registerConsumer(api::event_descriptor<test::Event>,
                                 [](api::IZiaMediator &, std::unique_ptr<api::IEvent>) {
                                     std::clog << "Consumer called" << std::endl;
                                 });
    return std::make_unique<test::Module>();
}

const api::EventDescriptor &test::Event::getDescriptor() const
{
    return api::event_descriptor<test::Event>;
}
