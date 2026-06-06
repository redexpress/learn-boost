add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE prj_lib Boost::unit_test_framework)
target_include_directories(test_main PRIVATE include)
add_test(NAME test_main COMMAND test_main)