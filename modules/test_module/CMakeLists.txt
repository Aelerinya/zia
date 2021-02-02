set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(zia_test_module SHARED
                source/main.cpp)

include_directories(../../interface/)
