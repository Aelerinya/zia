#include "Server.hpp"

const std::string name = "server";

Server::Server() {}

Server::~Server() {}

const std::string &Server::getName()const {
    return name;
}

void Server::configureModule(const YAML::Node &) {}

extern "C" std::unique_ptr<zia::api::IModule> zia::api::load_module(zia::api::IZiaInitializer &) {
    return std::make_unique<Server>();
}
