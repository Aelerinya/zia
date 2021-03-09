#ifndef ZIA_MODULE_HUB_H_
#define ZIA_MODULE_HUB_H_

#include <memory>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <ranges>
#include "ModuleProxy.hpp"


namespace zia {

class ModuleHub {
    using ModuleName = std::string_view;

public:
    void loadModule(const std::filesystem::path&);
    void unloadModule(ModuleName module);

    auto getModule(ModuleName)
        -> std::optional<std::reference_wrapper<api::IModule>>;

    auto getEventConsumer(const api::EventDescriptor&) const
        -> std::optional<std::reference_wrapper<const api::EventConsumer>>;

    auto getEventListeners(const api::EventDescriptor&) const
        -> std::vector<std::reference_wrapper<const api::EventListener>>;

public:
    struct Module {
        ModuleProxy proxy;

        struct Registrations {

            std::unordered_map<
                std::reference_wrapper<const api::EventDescriptor>,
                api::EventConsumer
            > consumers;

            std::unordered_multimap<
                std::reference_wrapper<const api::EventDescriptor>,
                api::EventListener
            > listeners;

        } registrations;
    };

private:
    std::unordered_map<ModuleName, Module> m_modules;
};

} // namespace zia


#endif // ZIA_MODULE_HUB_H_
