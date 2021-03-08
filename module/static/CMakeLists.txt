add_library(ziastatic MODULE
  static.cpp
  entrypoint.cpp)

include_directories(.)

target_link_libraries(ziastatic
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)

add_subdirectory(tests)
