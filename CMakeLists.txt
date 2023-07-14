cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 23)
project(SteamidPP VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_library(SteamidPP STATIC include/SteamID.hpp)
target_include_directories(SteamidPP PUBLIC include)
set_target_properties(SteamidPP PROPERTIES LINKER_LANGUAGE CXX)

enable_testing()
add_executable(tester test.cpp)
add_test(NAME SteamidPPTest COMMAND tester)
