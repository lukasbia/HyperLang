cmake_minimum_required(VERSION 3.25)

project(HyperLang
    VERSION 0.1.0
    DESCRIPTION "HyperLang programming language and native compiler"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HYPERLANG_ENABLE_TESTS "Build HyperLang tests" ON)
option(HYPERLANG_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(HYPERLANG_ENABLE_LSC "Build the Live Synchronized Compiler" ON)
option(HYPERLANG_ENABLE_MCM "Build Memory Cleanup Management" ON)

include(GNUInstallDirs)
include(CTest)

set(HYPERLANG_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(HYPERLANG_INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include)
set(HYPERLANG_LIBRARY_ROOT ${PROJECT_SOURCE_DIR}/lib)

if(HYPERLANG_ENABLE_WARNINGS)
    add_compile_options(
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endif()

add_subdirectory(include/hyperlang)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(libraries)

if(HYPERLANG_ENABLE_TESTS)
    add_subdirectory(tests)
endif()
