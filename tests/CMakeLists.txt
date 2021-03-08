include_directories(include)
add_subdirectory(source)

add_executable(test_zia
  test_zia.cpp
  test_module_proxy.cpp
  test_module_hub.cpp
  test_executor.cpp)

target_link_libraries(test_zia
  zialib ziatestlib)

add_test(NAME TestZia COMMAND test_zia)
