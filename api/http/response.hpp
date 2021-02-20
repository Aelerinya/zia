#ifndef ZIA_RESPONSE_H_
#define ZIA_RESPONSE_H_

#include "../event.hpp"

namespace zia::api
{
struct HTTPResponse {
    unsigned short status_code;
    std::string status_message;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class NewHTTPResponse : public IEvent
{
    virtual const HTTPResponse &getResponse = 0;
};
} // namespace zia::api
#endif // ZIA_RESPONSE_H_
