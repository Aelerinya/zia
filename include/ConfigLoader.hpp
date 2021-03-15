#ifndef ZIA_CONFIG_H_
#define ZIA_CONFIG_H_

#include "api/event.hpp"
#include "ModuleHub.hpp"
#include <yaml-cpp/yaml.h>

namespace zia {

struct ReloadConfig : api::IEvent {
    const api::EventDescriptor& getDescriptor() const override final;
};

namespace api {
template <>
constexpr EventDescriptor event_descriptor<ReloadConfig> = {"ReloadConfig"};
}

inline const api::EventDescriptor& ReloadConfig::getDescriptor() const {
        return api::event_descriptor<ReloadConfig>;
}

class ConfigLoader {

public:
    ConfigLoader(ModuleHub &hub) : m_module_hub(hub) {};
    explicit ConfigLoader(
        std::filesystem::path config_file,
        std::filesystem::path modules_dir,
        ModuleHub& module_hub
    );

    void reloadConfig();

private:
    std::filesystem::path m_config_file;
    YAML::Node m_current_config;
    std::filesystem::path m_modules_dir;
    ModuleHub& m_module_hub;
};


} // namespace zia

#endif // ZIA_CONFIG_H_
