#include <boost/asio.hpp>
#include <optional>

#include "HTTPParser.hpp"
#include "api/module.hpp"
#include "connection.hpp"
#include "response.hpp"

namespace zia::server
{

class Server
{
public:
    Server(api::IZiaMediator &mediator, short http_port = 80, short https_port = 443);
    ~Server();

    void waitForClientConnection();
    HTTPParser parser;
    api::IZiaMediator &mediator;

    void onHTTPResponse(std::unique_ptr<zia::server::NewHTTPResponse> response);
    void
    onHTTPConnection(std::unique_ptr<zia::server::NewHTTPConnectionEvent> connection);

private:
    std::thread context_thread;
    boost::asio::io_context asio_context;
    boost::asio::ip::tcp::acceptor web_acceptor;
    boost::asio::ip::tcp::acceptor websecure_acceptor;
};

class Module : public api::IModule
{
public:
    const std::string &getName() const final;
    void configureModule(const YAML::Node &) final;
    void start(zia::api::IZiaMediator &mediator);

    std::optional<std::reference_wrapper<Server>> getServer();

private:
    short http_port = 80;
    short https_port = 443;
    std::optional<Server> server;
};
}    // namespace zia::server
extern "C" std::unique_ptr<zia::api::IModule>
zia::api::load_module(zia::api::IZiaInitializer &);
