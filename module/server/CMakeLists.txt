add_library(ziaserver MODULE
  source/Server.cpp
  source/HTTPParser.cpp
  source/request.cpp
  source/response.cpp)

include_directories(include)

target_link_libraries(ziaserver
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp
  CONAN_PKG::magic_enum)
