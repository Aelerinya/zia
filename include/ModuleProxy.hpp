#ifndef ZIA_MODULE_PROXY_H_
#define ZIA_MODULE_PROXY_H_

#include <boost/dll.hpp>
#include <exception>
#include <filesystem>
#include <memory>

#include "api/module.hpp"

namespace zia
{
class ModuleProxy : public api::IModule
{
public:
    ModuleProxy(const std::filesystem::path &path, api::IZiaInitializer &initializer);

    const std::string &getName() const override;
    void configureModule(const YAML::Node &) override;

    class Error : public std::runtime_error
    {
    public:
        Error(const std::string &message, const std::filesystem::path &path);
    };

private:
    std::filesystem::path m_path;
    boost::dll::shared_library m_library;
    std::unique_ptr<api::IModule> m_module;
};

};    // namespace zia

#endif    // ZIA_MODULE_PROXY_H_
