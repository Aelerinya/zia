#include <iostream>
#include "Zia.hpp"

struct testModule : public virtual zia::protocol::INewClient {
    virtual bool onNewClient(const std::string &sRemoteIp, const unsigned uPort)final {
        std::cerr << sRemoteIp << ":" << uPort << std::endl;
        return true;
    }
};

extern "C" testModule *entrypoint(void) {
    return new testModule;
}
