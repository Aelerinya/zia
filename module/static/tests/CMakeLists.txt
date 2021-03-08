add_executable(test_static_handler
  test_static_handler.cpp
  ../static.cpp)

target_link_libraries(test_static_handler
  # ziatestlib
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)
add_test(NAME TestStaticHandler COMMAND test_static_handler)
