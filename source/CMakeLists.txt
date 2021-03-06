add_library(zialib
  ModuleProxy.cpp
  Logger.cpp)

target_link_libraries(zialib CONAN_PKG::boost dl)
