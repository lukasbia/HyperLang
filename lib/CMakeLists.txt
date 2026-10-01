add_library(hyper_parse
    Parse/Lexer.cpp
    Parse/LexerLiterals.cpp
    Parse/LexerOperators.cpp
    Parse/LexerUnicode.cpp
    Parse/LexerDiagnostics.cpp
    Parse/Token.cpp
    Parse/SourceLocation.cpp
)
target_include_directories(hyper_parse PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_features(hyper_parse PUBLIC cxx_std_20)
if(MSVC)
  target_compile_options(hyper_parse PRIVATE /W4 /permissive-)
else()
  target_compile_options(hyper_parse PRIVATE -Wall -Wextra -Wpedantic)
endif()
