#include <fstream>
#include <iostream>
#include <sstream>
#include <yaml-cpp/yaml.h>

#include "static.hpp"

using namespace zia;

namespace fs = std::filesystem;

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
    if (config["default"] && config["default"].IsSequence()) {
        for (auto &default_file: config["default"]) {
            if (!default_file.IsScalar())
                continue;
            auto filename = default_file.as<std::string>();
            m_default_files.push_back(filename);
            std::clog << "Default file name: " << filename << std::endl;
        }
    }
}

std::optional<std::filesystem::path> Static::findRessource(const api::http::Route &route)
{

    if (!m_base_directory.has_value())
        return std::nullopt;

    auto path =
        m_base_directory.value() / std::filesystem::path(route.path).relative_path();

    if (fs::is_directory(path)) {
        for (auto &default_file: m_default_files) {
            auto new_path = path / default_file;
            if (fs::exists(new_path))
                return new_path;
        }
        return std::nullopt;
    }

    return path;
}

api::http::HTTPResponse Static::handleRequest(const api::http::Route &route,
                                              const api::http::HTTPRequest &request)
{
    auto path = this->findRessource(route);

    if (!path.has_value())
        return notFound(request);

    std::ifstream f(*path);
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
