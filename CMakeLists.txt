cmake_minimum_required(VERSION 3.1.0)
project(r-type)

include(GNUInstallDirs)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Conan configuration

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_COMPILER ${CONAN_SETTINGS_COMPILER})
set(CONAN_LIBCXX ${CONAN_SETTINGS_COMPILER_LIBCXX})
set(CONAN_COMPILER_VERSION ${CONAN_SETTINGS_COMPILER_VERSION})
conan_basic_setup(TARGETS)

# link_libraries(${CONAN_LIBS})
include_directories(./include/)
include_directories(./)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(source)
add_subdirectory(module)
add_subdirectory(tests)

add_executable(zia source/main.cpp)

target_link_libraries(zia zialib CONAN_PKG::boost)
