cmake_minimum_required(VERSION 3.25)
project(HyperLang VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HYPERLANG_BUILD_TESTS "Build HyperLang tests" ON)

add_subdirectory(lib)
add_subdirectory(tools)

if(HYPERLANG_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
