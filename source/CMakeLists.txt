add_library(zialib
  ModuleProxy.cpp
  ModuleHub.cpp
  Executor.cpp)

target_link_libraries(zialib CONAN_PKG::boost dl)
