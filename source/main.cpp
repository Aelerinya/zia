#include <iostream>
#include "Library.hpp"
#include "Pipeline.hpp"
#include "Zia.hpp"

int main(void) {
    try {
        Library<zia::protocol::INewClient> mod("lib/libzia_test_module.so");

        mod->onNewClient("127.0.0.1", 2908);
    } catch (const LibraryErr &le) {
        std::cerr << le.getMessage() << std::endl;
    }
    return 0;
}
