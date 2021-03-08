#include "Server.hpp"
#include "connection.hpp"
#include "request.hpp"
#include "internal/onstart.hpp"
#include "response.hpp"
#include <iostream>
#include <string>

const std::string name = "server";

zia::server::Server::Server()
    : web_acceptor(asio_context,
                   boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 80)),
      websecure_acceptor(asio_context,
                         boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 443))
{
}

zia::server::Server::~Server()
{
}

const std::string &zia::server::Server::getName() const
{
    return name;
}

void zia::server::Server::configureModule(const YAML::Node &)
{
}

extern "C" std::unique_ptr<zia::api::IModule>
zia::api::load_module(zia::api::IZiaInitializer &init)
{
    auto serv = std::make_unique<zia::server::Server>();
    init.registerConsumer(
        zia::api::event_descriptor<zia::server::NewHTTPResponse>,
        [&serv](zia::api::IZiaMediator &m, std::unique_ptr<IEvent> ev) {
            const auto ht = dynamic_cast<zia::server::NewHTTPResponse *>(ev.get());
            const auto &rq = ht->getResponse();
            std::stringstream rp;
            rp << "HTTP/1.1 " << rq.status_code << " " << rq.status_message << "\r\n";
            for (const auto &[k, v]: rq.headers) {
                rp << k << ": " << v << "\r\n";
            }
            rp << "\r\n";
            rp << rq.body;
            boost::asio::async_write(ht->getSocket(),
                                     boost::asio::buffer(rp.str(), rp.str().size()),
                                     [&serv](std::error_code ec, std::size_t len) {
                                         if (ec) {
                                             std::clog << "Error while sending msg: "
                                                       << ec.message() << std::endl;
                                         }
                                     });
        });
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

                    auto p = std::make_unique<zia::server::NewHTTPRequest>(std::move(i), std::move(ht->socket));
                    if (serv->mediator) {
                        serv->mediator->get().emit(
                            static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(p)));
                    }
                });
        });
    return serv;
}

void zia::server::Server::init(zia::api::IZiaMediator &m)
{
    mediator = m;
    waitForClientConnection();
    context_thread = std::thread([this]() { asio_context.run(); });
}

void zia::server::Server::waitForClientConnection()
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
