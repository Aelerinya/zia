#include "ModuleProxy.hpp"

using namespace zia;
namespace fs = std::filesystem;

ModuleProxy::ModuleProxy(const fs::path &path, api::IZiaInitializer &initializer)
    : m_path(path),
      m_library(path.string(), boost::dll::load_mode::append_decorations),
      m_module(m_library.get<decltype(api::load_module)>("load_module")(initializer))
{
}

const std::string &ModuleProxy::getName() const
{
    return m_module->getName();
}

void ModuleProxy::configureModule(const YAML::Node &config)
{
    return m_module->configureModule(config);
}

ModuleProxy::Error::Error(const std::string &message, const std::filesystem::path &path)
    : std::runtime_error("Error in proxy of module " + path.string() + ": " + message)
{
}
