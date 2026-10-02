# HyperLang Parse subsystem.

add_library(HyperLangParse STATIC
  Lexer.cpp
)

target_include_directories(HyperLangParse
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangParse PUBLIC cxx_std_20)

add_library(HyperLangBasic STATIC
  ../Basic/UTF8.cpp
)

target_include_directories(HyperLangBasic
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangBasic PUBLIC cxx_std_20)

target_link_libraries(HyperLangParse PUBLIC HyperLangBasic)
