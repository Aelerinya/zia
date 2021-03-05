#include "HTTPParser.hpp"
#include "module.hpp"
#include <boost/asio.hpp>

class Server : public virtual zia::api::IModule
{
public:
    Server();
    ~Server();
    const std::string &getName() const final;
    void configureModule(const YAML::Node &) final;

    void init(zia::api::IZiaMediator &mediator);
    void waitForClientConnection();
    HTTPParser parser;
    std::optional<std::reference_wrapper<zia::api::IZiaMediator>> mediator;

private:
    std::thread context_thread;
    boost::asio::io_context asio_context;
    boost::asio::ip::tcp::acceptor web_acceptor;
    boost::asio::ip::tcp::acceptor websecure_acceptor;
};

extern "C" std::unique_ptr<zia::api::IModule>
zia::api::load_module(zia::api::IZiaInitializer &);
