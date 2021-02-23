#include "module.hpp"

class Server: public virtual zia::api::IModule {
public:
    Server();
    ~Server();
    virtual const std::string &getName()const;
    virtual void configureModule(const YAML::Node &);
};

extern "C" std::unique_ptr<zia::api::IModule> zia::api::load_module(zia::api::IZiaInitializer &);
