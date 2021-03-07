add_library(router MODULE
  source/router.cpp
  source/entrypoint.cpp)

include_directories(include)

target_link_libraries(router CONAN_PKG::boost)

add_subdirectory(tests)
