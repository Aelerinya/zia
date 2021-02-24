#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include "ThreadedQ.hpp"
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>

class Logger
{
public:
    Logger();
    ~Logger();
    void start();
    void stop();
    void flush();

    std::stringstream &warn(const std::string &msg = "WARNING");
    std::stringstream &err(const std::string &msg = "ERROR");
    std::stringstream &info(const std::string &msg = "INFO");
    std::stringstream &debug(const std::string &msg = "DEBUG");
    std::stringstream &msg(const std::string &msg = "MESSAGE");
    std::stringstream &raw();
    void endl();

private:
    std::mutex mutBuffer;
    std::atomic_bool bExit = false;
    std::thread msgT;
    ThreadedQ<std::string> qMsg;
    std::unordered_map<std::thread::id, std::stringstream> mBuffers;
};

#endif    //_LOGGER_HPP_

extern Logger logger;
