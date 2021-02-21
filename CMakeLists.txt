cmake_minimum_required(VERSION 3.1.0)
project(r-type)

include(GNUInstallDirs)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Conan configuration

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# link_libraries(${CONAN_LIBS})
include_directories(./include/)
include_directories(./api/)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(zia source/main.cpp)
target_link_libraries(zia CONAN_PKG::boost)
target_link_libraries(zia dl)

