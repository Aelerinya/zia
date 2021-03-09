#include <fstream>
#include <iostream>
#include <sstream>
#include <yaml-cpp/yaml.h>

#include "static.hpp"

using namespace zia;

namespace
{
api::http::HTTPResponse notFound(const api::http::HTTPRequest &request)
{
    return {404,
            "Not found",
            {},
            "Ressource " + request.route + " was not found on this server"};
}
}    // namespace

const std::string &Static::getName() const
{
    static const std::string name = "static";
    return name;
}

void Static::configureModule(const YAML::Node &config)
{
    m_base_directory = config["base_directory"].as<std::string>();
    std::clog << "Static module new base directory: " << m_base_directory.value()
              << std::endl;
}

api::http::HTTPResponse Static::handleRequest(const api::http::Route &route,
                                              const api::http::HTTPRequest &request)
{
    if (!m_base_directory.has_value())
        return notFound(request);

    auto path =
        m_base_directory.value() / std::filesystem::path(route.path).relative_path();
    std::ifstream f(path);
    if (!f)
        return notFound(request);

    if (request.method == api::http::HTTPMethod::HEAD)
        return {200, "OK", {}, {}};

    std::ostringstream oss;
    oss << f.rdbuf();
    return {200, "OK", {}, oss.str()};
}

const std::optional<std::filesystem::path> &Static::getBaseDirectory() const
{
    return m_base_directory;
}
