add_executable(test_static
  test_static.cpp
  ../static.cpp)

target_link_libraries(test_static
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)

add_test(NAME TestStatic COMMAND test_static)
