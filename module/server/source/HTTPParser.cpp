#include <iostream>
#include <magic_enum.hpp>
#include <regex>

#include "HTTPParser.hpp"

zia::api::http::HTTPRequest zia::server::HTTPParser::parse(const std::string &s)
{
    std::clog << "Parsing request " << s << std::endl;

    zia::api::http::HTTPRequest req;
    std::vector<std::string> line;
    std::string delimiter = "\r\n";
    std::regex crlf_reg("(\r\n)+");
    std::size_t start = 0;
    while (start != s.size()) {
        std::size_t pos = s.find(delimiter, start);
        std::string token = s.substr(start, pos - start);
        std::clog << "Request line: " << token << std::endl;
        start += token.size() + delimiter.size();

        if (!token.empty())
            line.push_back(token);
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
    std::clog << "Parsing request header " << s << std::endl;
    std::pair<zia::api::http::HTTPMethod, std::string> ret;
    std::regex method_path(R"#(^(GET|HEAD|TRACE|OPTIONS|PUT|PATCH)\s+(\S+)\s+(HTTP))#");
    std::smatch matches;
    if (std::regex_search(s, matches, method_path)) {
        auto e = magic_enum::enum_cast<zia::api::http::HTTPMethod>(matches[1].str());
        ret.first = *e;
        ret.second = matches[2];
    }
    return ret;
}
