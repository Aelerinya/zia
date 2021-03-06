#ifndef __HTTPPARSER_HPP__
#define __HTTPPARSER_HPP__

#include "http/request.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace zia::server
{
// could be constexpr
const std::unordered_set<std::string> std_http_header = {"A-IM",
                                                         "Accept",
                                                         "Accept-Charset",
                                                         "Accept-Datetime",
                                                         "Accept-Encoding",
                                                         "Accept-Language",
                                                         "Access-Control-Request-Method",
                                                         "Access-Control-Request-Headers",
                                                         "Authorization",
                                                         "Cache-Control",
                                                         "Connection",
                                                         "Content-Encoding",
                                                         "Content-Length",
                                                         "Content-MD5",
                                                         "Content-Type",
                                                         "Cookie",
                                                         "Date",
                                                         "Expect",
                                                         "Forwarded",
                                                         "From",
                                                         "Host",
                                                         "HTTP2-Settings",
                                                         "If-Match",
                                                         "If-Modified-Since",
                                                         "If-None-Match",
                                                         "If-Range",
                                                         "If-Unmodified-Since",
                                                         "Max-Forwards",
                                                         "Origin",
                                                         "Pragma",
                                                         "Prefer",
                                                         "Proxy-Authorization",
                                                         "Range",
                                                         "Referer",
                                                         "TE",
                                                         "Trailer",
                                                         "Transfer-Encoding",
                                                         "User-Agent",
                                                         "Upgrade",
                                                         "Via",
                                                         "Warning"};

class HTTPParser
{
public:
    HTTPParser() = default;
    ~HTTPParser() = default;

    zia::api::http::HTTPRequest parse(const std::string &);

private:
    std::pair<zia::api::http::HTTPMethod, std::string> parseHeader(const std::string &);
};
}    // namespace zia::server

#endif
