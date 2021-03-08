#ifndef ZIA_STATIC_H_
#define ZIA_STATIC_H_

#include <filesystem>

#include "api/http/response.hpp"
#include "api/http/routing.hpp"
#include "api/module.hpp"

namespace zia
{
class StaticHandler : public api::IModule
{
public:
    const std::string &getName() const override;
    void configureModule(const YAML::Node &) override;

    api::http::HTTPResponse handleRequest(const api::http::Route &,
                                          const api::http::HTTPRequest &);
    const std::optional<std::filesystem::path> &getBaseDirectory() const;

private:
    std::optional<std::filesystem::path> m_base_directory;
};
}    // namespace zia

#endif    // ZIA_STATIC_H_
