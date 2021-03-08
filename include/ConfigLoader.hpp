#ifndef ZIA_CONFIG_H_
#define ZIA_CONFIG_H_

#include "api/event.hpp"
#include "ModuleHub.hpp"
#include <yaml-cpp/yaml.h>

namespace zia {


struct ReloadConfig;

template <>
constexpr api::EventDescriptor api::event_descriptor<ReloadConfig> {"ReloadConfig"};

struct ReloadConfig : api::IEvent {
    const api::EventDescriptor& getDescriptor() const override final {
        return api::event_descriptor<ReloadConfig>;
    }
};


class ConfigLoader {

public:
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
