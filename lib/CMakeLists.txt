add_library(HyperLangCompiler
    Lexer/Lexer.cpp
    Lexer/LexerDiagnostics.cpp
    Lexer/Token.cpp
    Parser/Parser.cpp
    Parser/ParserDiagnostics.cpp
    AST/AST.cpp
    AST/ASTPrinter.cpp
    Sema/Sema.cpp
    Sema/TypeChecker.cpp
    HIL/HIL.cpp
    HIL/HILBuilder.cpp
    HIL/HIOptimizer.cpp
    LSC/LSC.cpp
    LSC/LSCSession.cpp
    MCM/MCM.cpp
    MCM/Ownership.cpp
    Diagnostics/DiagnosticEngine.cpp
    Support/SourceManager.cpp
)

target_include_directories(HyperLangCompiler PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(HyperLangCompiler PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(HyperLangCompiler PRIVATE /W4 /permissive-)
else()
    target_compile_options(HyperLangCompiler PRIVATE -Wall -Wextra -Wpedantic)
endif()
