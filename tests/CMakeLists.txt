add_executable(HyperLangLexerSmoke
  Lexer/LexerSmoke.cpp
)

target_include_directories(HyperLangLexerSmoke
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangLexerSmoke PRIVATE cxx_std_20)
target_link_libraries(HyperLangLexerSmoke PRIVATE HyperLangParse HyperLangBasic)

add_test(NAME HyperLangLexerSmoke COMMAND HyperLangLexerSmoke)
