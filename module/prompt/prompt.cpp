#include <iostream>

#include "prompt.hpp"
#include "ConfigLoader.hpp"
#include "api/mediator.hpp"

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

void Module::start(api::IZiaMediator &mediator)
{
    std::clog << "Launching prompt..." << std::endl;

    std::cin.sync_with_stdio(false);

    m_prompt_thread = std::jthread {
        [&](std::stop_token stopped) {
            while (!stopped.stop_requested()) {

                if (std::cin.rdbuf()->in_avail()) {
                    std::string line;
                    std::getline(std::cin, line);

                    if (line == "reload")
                        mediator.emit(std::make_unique<ReloadConfig>());
                    else
                        std::cerr << "Unknown command '" << line << "'\n";
                } else
                    std::this_thread::yield();
            }
        }
    };
}
