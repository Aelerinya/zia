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

enable_testing()

add_subdirectory(source)
add_subdirectory(module)
add_subdirectory(tests)

add_executable(zia source/main.cpp)
