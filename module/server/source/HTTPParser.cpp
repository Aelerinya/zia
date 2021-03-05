#include "HTTPParser.hpp"
#include <iostream>
#include <regex>

HTTPRequest HTTPParser::parse(const std::string &s)
{
    HTTPRequest req;
    std::deque<std::string> line;
    std::regex crlf_reg("(\r\n)+");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), crlf_reg);
    auto words_end = std::sregex_iterator();
    for (auto i = words_begin; i != words_end; ++i) {
        line.push_back(i->str());
    }
    req.head = this->parseHeader(line.at(0));
    std::regex header_path("^([\\w-]+): (.*)$");
    std::smatch matches;
    for (auto i = line.begin() + 1; i != line.end(); ++i) {
        if (std::regex_search(s, matches, header_path)) {
            if (!std_http_header.contains(matches[0]))
                std::clog << "Not a standars header" << std::endl;
            req.head.heads.insert({matches[0].str(), matches[1].str()});
        }
    }
    return req;
}

HTTPHeader HTTPParser::parseHeader(const std::string &s)
{
    HTTPHeader h;
    std::regex method_path("^(GET|HEAD|TRACE|OPTIONS|PUT|PATCH)\\s+(\\S+)<\\s+(HTTP)");
    std::smatch matches;
    if (std::regex_search(s, matches, method_path)) {
        h.verb = matches[0];
        h.path = matches[1];
    }
    return h;
}
