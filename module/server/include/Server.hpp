#include "module.hpp"
#include <boost/asio.hpp>

class Server: public virtual zia::api::IModule {
public:
    Server();
    ~Server();
    const std::string &getName()const final;
    void configureModule(const YAML::Node &)final;

    void init(const zia::api::IZiaMediator &mediator);
    void waitForClientConnection();

private:
    std::optional<std::reference_wrapper<const zia::api::IZiaMediator>> mediator;
    std::thread context_thread;
    boost::asio::io_context asio_context;
    boost::asio::ip::tcp::acceptor asio_acceptor;
};

extern "C" std::unique_ptr<zia::api::IModule> zia::api::load_module(zia::api::IZiaInitializer &);
