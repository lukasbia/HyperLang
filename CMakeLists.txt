cmake_minimum_required(VERSION 3.25)

project(HyperLang
    VERSION 0.2.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HYPERLANG_BUILD_TOOLS "Build HyperLang compiler tools" ON)

add_subdirectory(lib)

if(HYPERLANG_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

include(GNUInstallDirs)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

message(STATUS "HyperLang compiler pipeline:")
message(STATUS "  Source -> Lexer -> Parser -> Sema -> HIL")
message(STATUS "  HIL -> HIL Optimization -> LSC/LLCVM -> Backend")
