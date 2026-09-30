add_library(HyperLangCompiler
    Lexer/Lexer.cpp
    Lexer/Token.cpp
    Parser/Parser.cpp
    Sema/Sema.cpp
    HIL/HIL.cpp
    LSC/LSC.cpp
    MCM/MCM.cpp
    Diagnostics/DiagnosticEngine.cpp
    Support/SourceManager.cpp
)

target_include_directories(HyperLangCompiler PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperLangCompiler PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(HyperLangCompiler PRIVATE /W4 /permissive-)
else()
    target_compile_options(HyperLangCompiler PRIVATE -Wall -Wextra -Wpedantic)
endif()
