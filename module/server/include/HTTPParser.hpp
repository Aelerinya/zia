#ifndef __HTTPPARSER_HPP__
#define __HTTPPARSER_HPP__

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

enum class HTTPVerbs { GET, POST, PUT, DELETE, PATCH, OPTIONS, TRACE, CONNECT, HEAD };

struct HTTPHeader {
    std::string verb;
    std::string path;
    std::unordered_map<std::string, std::string> heads;
};

struct HTTPRequest {
    HTTPHeader head;
    std::vector<uint8_t> body;
};

class HTTPParser
{
public:
    HTTPParser() = default;
    ~HTTPParser() = default;

    HTTPRequest parse(const std::string &);

private:
    HTTPHeader parseHeader(const std::string &);
};

#endif
