#include <iostream>
#include "ModuleHub.hpp"
#include "api/initializer.hpp"

namespace zia {


void ModuleHub::loadModule(const std::filesystem::path& path)
{
    struct Initializer : api::IZiaInitializer, Module::Registrations {

        void registerConsumer(
            const api::EventDescriptor& event,
            api::EventConsumer consumer
        ) override {
            std::clog << "Registering consumer for event '" << event.name << "'\n";
            consumers.emplace(event, std::move(consumer));
        }

        void registerListener(
            const api::EventDescriptor& event,
            api::EventListener listener
        ) override {
            std::clog << "Registering listener for event '" << event.name << "'\n";
            listeners.emplace(event, std::move(listener));
        }

    } initializer;


    std::clog << "Loading module '" << path.filename().string() << "'\n";
    ModuleProxy proxy { path, initializer };
    const auto& name { proxy.getName() };

    const auto [module, success] {
        m_modules.try_emplace(name, std::move(proxy), std::move(initializer))
    };

    if (!success) {
        const auto& [consumers, listeners] { module->second.registrations };

        std::cerr << "ERROR: module '" << name << "' already loaded, consuming events";
        for (const auto& [event, _] : consumers) std::cerr << " '" << event.get().name << "'";
        std::cerr << " and listening to events";
        for (const auto& [event, _] : listeners) std::cerr << " '" << event.get().name << "'";
        std::cerr << ". To unload it, call `ModuleHub::unloadModule(\"" << name << "\")`.\n";
    }
}

void ModuleHub::unloadModule(ModuleName name)
{
    std::clog << "Unloading module '" << name << "'\n";
    if (m_modules.erase(name) == 0) {
        std::cerr << "ERROR: cannot unload '" << name << "', module not found.\n";
    }
}

auto ModuleHub::getModule(const ModuleName name)
    -> std::optional<std::reference_wrapper<api::IModule>>
{
    const auto module { m_modules.find(name) };

    if (module == m_modules.end()) return {};
    return module->second.proxy;
}


auto ModuleHub::getEventConsumer(const api::EventDescriptor& event) const
    -> std::optional<std::reference_wrapper<const api::EventConsumer>>
{
    using namespace std::views;

    const auto consumerss {
        m_modules | values | transform(
            [](const auto& module) -> const auto& { return module.registrations.consumers; }
        )
    };

    for (const auto& consumers : consumerss)
        for (const auto& consumer : consumers)
            if (consumer.first == event)
                return consumer.second;

    return {};
}

auto ModuleHub::getEventListeners(const api::EventDescriptor& event) const
    -> std::vector<std::reference_wrapper<const api::EventListener>>
{
    std::vector<std::reference_wrapper<const api::EventListener>> event_listeners;

    using namespace std::views;

    auto listenerss {
        m_modules | values | transform(
            [](const auto& module) -> const auto& { return module.registrations.listeners; }
        )
    };

    for (const auto& listeners : listenerss)
        for (const auto& listener : listeners)
            if (listener.first == event)
                event_listeners.emplace_back(listener.second);

    return event_listeners;
}


} // namespace zia
