#include <iostream>
#include "ModuleHub.hpp"
#include "api/initializer.hpp"

namespace zia {


void ModuleHub::loadModule(const std::filesystem::path path)
{
    struct Initializer : api::IZiaInitializer, Module::Registrations {

        void registerConsumer(
            const api::EventDescriptor& event,
            api::EventConsumer consumer
        ) override {
            consumers.emplace(event, std::move(consumer));
        }

        void registerListener(
            const api::EventDescriptor& event,
            api::EventListener listener
        ) override {
            listeners.emplace(event, std::move(listener));
        }

    } initializer;


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

    const auto consumers {
        m_modules | values | transform(
            [](const auto& module) -> const auto& { return module.registrations.consumers; }
        ) | join
    };

    const auto consumer {
        std::ranges::find_if(consumers,
            [&](const auto& consumer) { return consumer.first == event; }
        )
    };

    if (consumer == consumers.end()) return {};
    return (*consumer).second;
}

auto ModuleHub::getEventListeners(const api::EventDescriptor& event) const
    -> std::vector<std::reference_wrapper<const api::EventListener>>
{
    using namespace std::views;

    auto listeners {
        m_modules | values | transform(
            [](const auto& module) -> const auto& { return module.registrations.listeners; }
        ) | join | filter(
            [&](const auto& listener) { return listener.first == event; }
        ) | values | common
    };

    return { listeners.begin(), listeners.end() };
}


} // namespace zia
