#include <iostream>

#include "prompt.hpp"

using namespace zia::prompt;
using namespace zia;

const std::string &Module::getName() const
{
    static const std::string name = "prompt";
    return name;
}

void Module::configureModule([[maybe_unused]] const YAML::Node &config)
{
}

void Module::start(api::IZiaMediator &)
{
    std::clog << "Launching prompt..." << std::endl;
 // TODO: laucnh prompt
}
