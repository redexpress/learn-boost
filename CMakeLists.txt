cmake_minimum_required(VERSION 3.28)
project(learn_boost)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.83 REQUIRED COMPONENTS json unit_test_framework log log_setup)
find_package(OpenSSL REQUIRED)

include(FetchContent)
FetchContent_Declare(
        tl_expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(tl_expected)

option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

enable_testing()
add_subdirectory(tests)

add_library(prj_lib
        src/sort.cpp
        src/net.cpp
        src/lib/network.cpp
        src/json.cpp
        src/devcore.cpp
        src/lib/Base64.cpp
        src/lib/AES.cpp
        src/lib/Hash.cpp
        src/lib/Log.cpp
)

add_executable(learn_boost src/main.cpp)

target_include_directories(prj_lib PUBLIC include src ${Boost_INCLUDE_DIRS})
target_include_directories(learn_boost PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(prj_lib PUBLIC OpenSSL::Crypto Boost::json Boost::log Boost::log_setup tl::expected)
target_link_libraries(learn_boost prj_lib)
if (NOT WIN32)
    target_link_libraries(learn_boost m)
endif()