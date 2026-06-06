cmake_minimum_required(VERSION 3.28)
project(learn_boost)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.83 REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(prj_lib
        src/sort.cpp
        src/net.cpp
        src/lib/network.cpp
)

add_executable(learn_boost src/main.cpp)

target_include_directories(prj_lib PUBLIC include src)
target_include_directories(learn_boost PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(prj_lib PUBLIC OpenSSL::Crypto)
target_link_libraries(learn_boost prj_lib)
if (NOT WIN32)
    target_link_libraries(learn_boost m)
endif()


