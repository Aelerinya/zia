#include "Server.hpp"
#include "http/connection.hpp"
#include "internal/onstart.hpp"

const std::string name = "server";

Server::Server(): asio_acceptor(asio_context,
                                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 80)) {}

Server::~Server() {}

const std::string &Server::getName()const {
    return name;
}

void Server::configureModule(const YAML::Node &) {}

extern "C" std::unique_ptr<zia::api::IModule> zia::api::load_module(zia::api::IZiaInitializer &init) {
    auto serv = std::make_unique<Server>();
    auto onStart = zia::api::OnStartEvent();
    init.registerListener(onStart.getDescriptor(), [&serv](const zia::api::IZiaMediator &m, const IEvent &) {
        serv->init(m);
    });
    return std::move(serv);
}

void Server::init(const zia::api::IZiaMediator &m) {
    mediator = m;
    waitForClientConnection();
    context_thread = std::thread([this]() { asio_context.run(); } );
}

void Server::waitForClientConnection() {
    asio_acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket) {
            zia::api::NewHTTPConnectionEvent con(asio_context, std::move(socket));
       }
    );
}
