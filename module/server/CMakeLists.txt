set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(zia_server SHARED
            source/Server.cpp
            source/HTTPParser.cpp)

include_directories(include)
include_directories(../../api/)

target_link_libraries(zia_server CONAN_PKG::boost)
