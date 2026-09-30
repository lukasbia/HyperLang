cmake_minimum_required(VERSION 3.25)
project(HyperLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(libraries)
add_subdirectory(tests)
