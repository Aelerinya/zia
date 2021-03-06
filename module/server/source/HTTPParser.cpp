#include "HTTPParser.hpp"
#include <iostream>
#include <magic_enum.hpp>
#include <regex>

zia::api::http::HTTPRequest zia::server::HTTPParser::parse(const std::string &s)
{
    zia::api::http::HTTPRequest req;
    std::deque<std::string> line;
    std::regex crlf_reg("(\r\n)+");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), crlf_reg);
    auto words_end = std::sregex_iterator();
    for (auto i = words_begin; i != words_end; ++i) {
        line.push_back(i->str());
    }
    auto i = this->parseHeader(line.at(0));
    req.method = std::move(i.first);
    req.route = std::move(i.second);
    std::regex header_path("^([\\w-]+): (.*)$");
    std::smatch matches;
    for (auto i = line.begin() + 1; i != line.end(); ++i) {
        if (std::regex_search(s, matches, header_path)) {
            if (!std_http_header.contains(matches[0]))
                std::clog << "Not a standars header" << std::endl;
            req.headers.insert({matches[0].str(), matches[1].str()});
        }
    }
    return req;
}

std::pair<zia::api::http::HTTPMethod, std::string>
zia::server::HTTPParser::parseHeader(const std::string &s)
{

    std::pair<zia::api::http::HTTPMethod, std::string> ret;
    std::regex method_path("^(GET|HEAD|TRACE|OPTIONS|PUT|PATCH)\\s+(\\S+)<\\s+(HTTP)");
    std::smatch matches;
    if (std::regex_search(s, matches, method_path)) {
        auto e = magic_enum::enum_cast<zia::api::http::HTTPMethod>(matches[0].str());
        ret.first = *e;
        ret.second = matches[1];
    }
    return ret;
}
