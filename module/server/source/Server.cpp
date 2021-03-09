#include "Server.hpp"
#include "api/internal/onstart.hpp"
#include "connection.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>

const std::string name = "server";

const std::string &zia::server::Module::getName() const
{
    return name;
}

void zia::server::Module::configureModule(const YAML::Node &node)
{
    // TODO: Restart server if ports changed
    if (node["http"])
        this->http_port = node["http"].as<short>(80);
    if (node["https"])
        this->https_port = node["https"].as<short>(443);
}

void zia::server::Module::start(zia::api::IZiaMediator &mediator)
{
    std::clog << "Starting server ..." << std::endl;
    std::clog << "http port " << this->http_port << std::endl;
    std::clog << "https port " << this->https_port << std::endl;
    this->server.emplace(mediator, this->http_port, this->https_port);
}

std::optional<std::reference_wrapper<zia::server::Server>>
zia::server::Module::getServer()
{
    if (this->server.has_value())
        return {{this->server.value()}};
    else
        return std::nullopt;
}

zia::server::Server::Server(api::IZiaMediator &mediator, short http_port,
                            short https_port)
    : mediator(mediator),
      web_acceptor(asio_context,
                   boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), http_port)),
      websecure_acceptor(asio_context, boost::asio::ip::tcp::endpoint(
                                           boost::asio::ip::tcp::v4(), https_port))
{
    std::clog << "Starting server" << std::endl;
    waitForClientConnection();
    context_thread = std::thread([this]() { asio_context.run(); });
}

zia::server::Server::~Server()
{
    asio_context.stop();
    if (context_thread.joinable()) {
        context_thread.join();
    };
}

void zia::server::Server::onHTTPResponse(
    std::unique_ptr<zia::server::NewHTTPResponse> response)
{
    const auto &rq = response->getResponse();
    std::stringstream rp;
    rp << "HTTP/1.1 " << rq.status_code << " " << rq.status_message << "\r\n";
    for (const auto &[k, v]: rq.headers) {
        rp << k << ": " << v << "\r\n";
    }
    rp << "\r\n";
    rp << rq.body;
    boost::asio::async_write(
        response->getSocket(), boost::asio::buffer(rp.str(), rp.str().size()),
        [](std::error_code ec, std::size_t) {
            if (ec) {
                std::clog << "Error while sending msg: " << ec.message() << std::endl;
            }
        });
}

void zia::server::Server::onHTTPConnection(
    std::unique_ptr<zia::server::NewHTTPConnectionEvent> connection)
{

    auto buf = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(
        connection->socket, *buf, "\r\n\r\n",
        [buf, this, connection = std::move(connection)](std::error_code,
                                                        std::size_t len) {
            auto data = buf->data();
            buf->consume(len);
            std::string s(boost::asio::buffers_begin(data), buffers_begin(data) + len);
            auto i = this->parser.parse(s);
            boost::asio::async_read(connection->socket, boost::asio::buffer(i.body),
                                    boost::asio::transfer_all(),
                                    [&](std::error_code, std::size_t) {});

            auto p = std::make_unique<zia::server::NewHTTPRequest>(
                std::move(i), std::move(connection->socket));
            this->mediator.emit(
                static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(p)));
        });
}

extern "C" std::unique_ptr<zia::api::IModule>
zia::api::load_module(zia::api::IZiaInitializer &init)
{
    auto mod = std::make_unique<zia::server::Module>();
    init.registerListener(
        zia::api::event_descriptor<zia::api::OnStartEvent>,
        [&mod = *mod](zia::api::IZiaMediator &m, const IEvent &) { mod.start(m); });

    init.registerConsumer(
        zia::api::event_descriptor<zia::api::http::NewHTTPResponse>,
        [&mod = *mod](zia::api::IZiaMediator &, std::unique_ptr<IEvent> ev) {
            auto event =
                zia::dynamic_unique_ptr_cast<zia::server::NewHTTPResponse>(std::move(ev));
            auto server = mod.getServer();
            if (server.has_value())
                server->get().onHTTPResponse(std::move(event));
        });

    init.registerConsumer(
        zia::api::event_descriptor<zia::api::http::NewHTTPConnectionEvent>,
        [&mod = *mod](zia::api::IZiaMediator &, std::unique_ptr<IEvent> ev) {
            auto event =
                zia::dynamic_unique_ptr_cast<zia::server::NewHTTPConnectionEvent>(
                    std::move(ev));
            auto server = mod.getServer();
            if (server.has_value())
                server->get().onHTTPConnection(std::move(event));
        });
    return mod;
}

void zia::server::Server::waitForClientConnection()
{
    web_acceptor.async_accept(
        [this](std::error_code, boost::asio::ip::tcp::socket socket) {
            auto con = std::make_unique<zia::server::NewHTTPConnectionEvent>(
                asio_context, std::move(socket));
            mediator.emit(static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
            this->waitForClientConnection();
        });
    websecure_acceptor.async_accept(
        [this](std::error_code, boost::asio::ip::tcp::socket socket) {
            auto con = std::make_unique<zia::server::NewHTTPSConnectionEvent>(
                asio_context, std::move(socket));
            mediator.emit(static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
            this->waitForClientConnection();
        });
}
