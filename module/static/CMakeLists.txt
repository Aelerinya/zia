add_library(static MODULE
  static.cpp
  entrypoint.cpp)

include_directories(.)

target_link_libraries(static CONAN_PKG::boost)

add_subdirectory(tests)
