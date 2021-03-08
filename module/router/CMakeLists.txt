add_library(ziarouter MODULE
  source/router.cpp
  source/entrypoint.cpp)

include_directories(include)

target_link_libraries(ziarouter
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)

add_subdirectory(tests)
