#include "ConfigLoader.hpp"

namespace zia {


ConfigLoader::ConfigLoader(
    std::filesystem::path config_file,
    std::filesystem::path modules_dir,
    ModuleHub& module_hub
)
    : m_config_file { std::move(config_file) }
    , m_modules_dir { std::move(modules_dir) }
    , m_module_hub { module_hub }
{
    if (!std::filesystem::is_regular_file(m_config_file))
        throw std::runtime_error {
            '\'' + m_config_file.string() + "' is not a regular file"
        };

    if (!std::filesystem::is_directory(m_modules_dir))
        throw std::runtime_error {
            '\'' + m_modules_dir.string() + "' is not a directory"
        };

    reloadConfig();
}


void ConfigLoader::reloadConfig()
{
    for (const auto module_config : m_current_config) {
        m_module_hub.unloadModule(module_config.first.as<std::string>());
    }

    m_current_config = YAML::LoadFile(m_config_file.string());
    const auto modules { m_current_config["modules"] };
    m_current_config.remove("modules");

    for (const auto module_config : m_current_config) {
        const auto module_name { module_config.first.as<std::string>() };

        const auto module_lib {
            modules && modules[module_name] ?
                modules[module_name].as<std::string>() : module_name
        };

        m_module_hub.loadModule(m_modules_dir / module_lib);
        const auto module { m_module_hub.getModule(module_name) };

        if (!module)
            throw std::runtime_error {
                "Invalid module '" + module_name + "': "
                "`getName()` does not return \"" + module_name + '"'
            };

        module->get().configureModule(module_config.second);
    }
}


} // namespace zia
