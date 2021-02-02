#ifndef _ZIA_HPP_
#define _ZIA_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "Pipeline.hpp"

namespace zia
{

struct RawRequest {
    const std::string sSourceIp;
    const unsigned uSourcePort;
    const std::vector<std::byte> vRawRequest;
};

enum class Protocol { HTTPS, HTTP };

struct HttpRequest {
    const std::string path;
    const enum { GET, PUSH, PUT, DELETE, PATCH } http;
    const std::unordered_map<std::string, std::string> header;
    const std::vector<std::byte> body;
};

struct HttpsRequest {
    std::vector<std::byte> decypheredRequest;
};

struct RequestGlobal {
    const RawRequest info;
    Protocol protocol;
};

namespace protocol
{
    class INewClient {
        public:
            virtual bool onNewClient(const std::string &sRemoteIp, const unsigned uPort) = 0;
    };

    class INewMessage {
        public:
            virtual bool onNewMessage(const RawRequest &rawRequest) = 0;
    };
}

}    // namespace zia

#endif    //_ZIA_HPP_
