#define BOOST_TEST_MODULE Router
#include <boost/test/included/unit_test.hpp>
#include <yaml-cpp/yaml.h>

#include "static.hpp"

namespace fs = std::filesystem;

BOOST_AUTO_TEST_CASE(test_static_handler)
{
    fs::path tmp_dir = fs::temp_directory_path();
    std::string tmp_file_name = "zia_static_handler_test_file";
    fs::path tmp_file_path = tmp_dir / tmp_file_name;

    std::ofstream f(tmp_file_path, std::ios_base::out | std::ios_base::trunc);
    BOOST_TEST((bool)f);

    std::string_view test_text = "some text";
    f << test_text;
    f.close();

    YAML::Node config = YAML::Load("base_directory: " + tmp_dir.string());
    BOOST_TEST(config["base_directory"].as<std::string>() == tmp_dir.string());

    zia::StaticHandler handler;

    BOOST_TEST(!handler.getBaseDirectory().has_value());
    handler.configureModule(config);
    BOOST_TEST(handler.getBaseDirectory().value() == tmp_dir.string());


    zia::api::http::Route route{tmp_file_name, {}};
    zia::api::http::HTTPRequest request{
        zia::api::http::HTTPMethod::GET, "/somewhere/" + tmp_file_name, {}, ""};

    zia::api::http::HTTPResponse response = handler.handleRequest(route, request);
    BOOST_TEST(response.status_code == 200);
    BOOST_TEST(response.status_message == "OK");
    BOOST_TEST(response.headers.size() == 0);
    BOOST_TEST(response.body == test_text);

    // File not found
    route.path = "does_not_exist";
    response = handler.handleRequest(route, request);
    BOOST_TEST(response.status_code == 404);
    BOOST_TEST(response.status_message == "Not found");
    BOOST_TEST(response.headers.size() == 0);
}
