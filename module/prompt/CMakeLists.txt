add_library(ziaprompt MODULE
  prompt.cpp
  entrypoint.cpp)

include_directories(.)

target_link_libraries(ziaprompt
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)

# add_subdirectory(tests)
