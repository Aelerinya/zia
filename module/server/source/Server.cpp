#include "Server.hpp"
#include "connection.hpp"
#include "internal/onstart.hpp"

const std::string name = "server";

Server::Server()
    : web_acceptor(asio_context,
                   boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 80)),
      websecure_acceptor(asio_context,
                         boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 443))
{
}

Server::~Server()
{
}

const std::string &Server::getName() const
{
    return name;
}

void Server::configureModule(const YAML::Node &)
{
}

extern "C" std::unique_ptr<zia::api::IModule>
zia::api::load_module(zia::api::IZiaInitializer &init)
{
    auto serv = std::make_unique<Server>();
    init.registerConsumer(
        zia::api::event_descriptor<zia::api::OnStartEvent>,
        [&serv](zia::api::IZiaMediator &m, std::unique_ptr<IEvent>) { serv->init(m); });
    init.registerConsumer(
        zia::api::event_descriptor<zia::api::NewHTTPConnectionEvent>,
        [&serv](zia::api::IZiaMediator &, std::unique_ptr<IEvent> ev) {
            auto ht = dynamic_cast<NewHTTPConnectionEvent *>(ev.get());
            boost::asio::streambuf buf;
            boost::asio::async_read_until(
                ht->socket, buf, "\r\n\r\n", [&](std::error_code, std::size_t len) {
                    auto data = buf.data();
                    buf.consume(len);
                    std::string s(boost::asio::buffers_begin(data),
                                  buffers_begin(data) + len);
                    auto i = serv->parser.parse(s);
                    boost::asio::async_read(ht->socket, boost::asio::buffer(i.body),
                                            boost::asio::transfer_all(),
                                            [&](std::error_code, std::size_t) {});

                    auto p = std::make_unique<zia::api::HTTPRequestParsed>(std::move(i));
                    if (serv->mediator) {
                        serv->mediator->get().emit(
                            static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(p)));
                    }
                });
        });
    return serv;
}

void Server::init(zia::api::IZiaMediator &m)
{
    mediator = m;
    waitForClientConnection();
    context_thread = std::thread([this]() { asio_context.run(); });
}

void Server::waitForClientConnection()
{
    web_acceptor.async_accept([this](std::error_code,
                                     boost::asio::ip::tcp::socket socket) {
        auto con = std::make_unique<zia::api::NewHTTPConnectionEvent>(asio_context,
                                                                      std::move(socket));
        if (mediator)
            mediator->get().emit(
                static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
        this->waitForClientConnection();
    });
    websecure_acceptor.async_accept([this](std::error_code,
                                           boost::asio::ip::tcp::socket socket) {
        auto con = std::make_unique<zia::api::NewHTTPSConnectionEvent>(asio_context,
                                                                       std::move(socket));
        if (mediator)
            mediator->get().emit(
                static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
        this->waitForClientConnection();
    });
}
