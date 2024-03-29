cmake_minimum_required(VERSION 3.1.0)
project(zia)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Conan configuration

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_COMPILER ${CONAN_SETTINGS_COMPILER})
set(CONAN_LIBCXX ${CONAN_SETTINGS_COMPILER_LIBCXX})
set(CONAN_COMPILER_VERSION ${CONAN_SETTINGS_COMPILER_VERSION})
conan_basic_setup(TARGETS)

include_directories(.)
include_directories(include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32) # Avoid double inclusion and remove useless warnings on Windows
  add_compile_options(/wd4190 /FIsdkddkver.h /experimental:external
    /external:anglebrackets /external:W0 /external:templates-)
  add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()

enable_testing()

add_subdirectory(source)
add_subdirectory(module)
add_subdirectory(tests)

add_executable(zia source/main.cpp)
target_link_libraries(zia zialib)
