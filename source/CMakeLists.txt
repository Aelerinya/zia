add_library(zialib OBJECT
  ModuleProxy.cpp
  ModuleHub.cpp
  Executor.cpp)

target_link_libraries(zialib
  CONAN_PKG::boost ${CMAKE_DL_LIBS}
  CONAN_PKG::yaml-cpp)
