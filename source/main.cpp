#include <iostream>
#include "ConfigLoader.hpp"

int main(int argc, const char* const* argv) try
{
    if (argc != 3)
        throw std::runtime_error {
            "Invalid number of arguments (got " +
            std::to_string(argc) + ", expected 2)\n"
            "Usage: ./zia CONFIG_FILE MODULES_DIR"
        };

    zia::ModuleHub module_hub;
    zia::ConfigLoader config_loader { argv[1], argv[2], module_hub };
}

catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << '\n';
    return 84;
}
