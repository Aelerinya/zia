#include "Server.hpp"
#include "connection.hpp"
#include "internal/onstart.hpp"

const std::string name = "server";

Server::Server(): web_acceptor(asio_context,
                                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 80)),
                                websecure_acceptor(asio_context,
                                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 443)) {}

Server::~Server() {}

const std::string &Server::getName()const {
    return name;
}

void Server::configureModule(const YAML::Node &) {}

extern "C" std::unique_ptr<zia::api::IModule> zia::api::load_module(zia::api::IZiaInitializer &init) {
    auto serv = std::make_unique<Server>();
    auto onStart = zia::api::OnStartEvent();
    init.registerConsumer(onStart.getDescriptor(), [&serv](zia::api::IZiaMediator &m, const IEvent &) {
        serv->init(m);
    });
    return std::move(serv);
}

void Server::init(zia::api::IZiaMediator &m) {
    mediator = m;
    waitForClientConnection();
    context_thread = std::thread([this]() { asio_context.run(); } );
}

void Server::waitForClientConnection() {
    web_acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket) {
            auto con = std::make_unique<zia::api::NewHTTPConnectionEvent>(asio_context, std::move(socket));
            if (mediator) mediator->get().emit(static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
            this->waitForClientConnection();
       }
    );    websecure_acceptor.async_accept(
        [this](std::error_code ec, boost::asio::ip::tcp::socket socket) {
            auto con = std::make_unique<zia::api::NewHTTPSConnectionEvent>(asio_context, std::move(socket));
            if (mediator) mediator->get().emit(static_cast<std::unique_ptr<zia::api::IEvent>>(std::move(con)));
            this->waitForClientConnection();
       }
    );
}
