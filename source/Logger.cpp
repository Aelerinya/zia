#include "Logger.hpp"

Logger::Logger() {}

Logger::~Logger() { this->stop(); }

void Logger::start()
{
    msgT = std::thread([this]() {
        while (!bExit) {
            try {
                qMsg.wait();
                while (!qMsg.empty()) {
                    std::cerr << qMsg.pop_front() << "\e[0m" << std::endl;
                }
            } catch (const std::exception &e) {
                std::cerr << "LOGGER ERROR:" << e.what() << std::endl;
            }
        }
    });
}

void Logger::stop()
{
    this->flush();
    bExit = true;
    qMsg.setWaitMode(false);
    if (msgT.joinable()) { msgT.join(); }
}

void Logger::flush()
{
    std::unique_lock<std::mutex> lBuffers;

    for (auto &[_, i]: mBuffers) {
        std::string msg(i.str());
        if (!msg.empty()) std::cerr << msg << std::endl;
        i = std::stringstream();
    }
}

void Logger::endl()
{
    std::string msg(mBuffers.at(std::this_thread::get_id()).str());
    qMsg.push_back(msg);
    mBuffers.at(std::this_thread::get_id()) = std::stringstream();
}

std::stringstream &Logger::warn(const std::string &msg)
{
    auto &buf = this->raw();
    buf << "\e[33m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::err(const std::string &msg)
{
    auto &buf = this->raw();
    buf << "\e[31m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::info(const std::string &msg)
{
    auto &buf = this->raw();
    buf << "[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::debug(const std::string &msg)
{
    auto &buf = this->raw();
    buf << "\e[36m[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::msg(const std::string &msg)
{
    auto &buf = this->raw();
    buf << "[" << msg << "]: ";
    return buf;
}

std::stringstream &Logger::raw()
{
    if (!mBuffers.contains(std::this_thread::get_id())) {
        mBuffers.insert({std::this_thread::get_id(), std::stringstream()});
    }
    return mBuffers.at(std::this_thread::get_id());
}
