add_executable(test_router
  test_router.cpp
  ../source/router.cpp)

target_link_libraries(test_router
  ziatestlib
  CONAN_PKG::boost
  CONAN_PKG::yaml-cpp)
add_test(NAME TestRouter COMMAND test_router)
