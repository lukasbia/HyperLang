# HyperLang Parse subsystem.

add_library(HyperLangParse STATIC
  Lexer.cpp
  Parser.cpp
  ../../include/hyperlang/Parse/AST.h
  ../../include/hyperlang/Parse/Parser.h
  ../../include/hyperlang/Parse/ParserDiagnostics.h
  ../../include/hyperlang/Parse/Lexer.h
  ../../include/hyperlang/Parse/LexerDiagnostics.h
  ../../include/hyperlang/Parse/LexerKeywords.h
  ../../include/hyperlang/Parse/LexerLiterals.h
  ../../include/hyperlang/Parse/LexerOperators.h
  ../../include/hyperlang/Parse/LexerOptions.h
  ../../include/hyperlang/Parse/LexerToken.h
  ../../include/hyperlang/Parse/LexerTrivia.h
  ../../include/hyperlang/Parse/LexerUnicode.h
  ../../include/hyperlang/Parse/TokenKinds.h
)

target_include_directories(HyperLangParse
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangParse PUBLIC cxx_std_20)

add_library(HyperLangBasic STATIC
  ../Basic/UTF8.cpp
  ../../include/hyperlang/Basic/UTF8.h
)

target_include_directories(HyperLangBasic
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangBasic PUBLIC cxx_std_20)
target_link_libraries(HyperLangParse PUBLIC HyperLangBasic)
