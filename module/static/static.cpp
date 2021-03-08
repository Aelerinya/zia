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
            "Ressource " + request.route + "was not found on this server"};
}
}    // namespace

const std::string &StaticHandler::getName() const
{
    static const std::string name = "StaticHandler";
    return name;
}

void StaticHandler::configureModule(const YAML::Node &config)
{
    m_base_directory = config["base_directory"].as<std::string>();
    std::clog << "Static handler new base directory: " << m_base_directory.value()
              << std::endl;
}

api::http::HTTPResponse
StaticHandler::handleRequest(const api::http::Route &route,
                             const api::http::HTTPRequest &request)
{
    if (!m_base_directory.has_value())
        return notFound(request);

    std::ifstream f(m_base_directory.value() / route.path);
    if (!f)
        return notFound(request);

    std::ostringstream oss;
    oss << f.rdbuf();
    return {200, "OK", {}, oss.str()};
}

const std::optional<std::filesystem::path> &StaticHandler::getBaseDirectory() const
{
    return m_base_directory;
}
