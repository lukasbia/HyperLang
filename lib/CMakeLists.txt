add_library(HyperLangCompiler
    Driver/Driver.cpp
    Parse/Lexer/Lexer.cpp
    Lexer/Token.cpp
    Parse/Parser/Parser.cpp
    Sema/Sema.cpp
    HIL/HIL.cpp
    HIL/Optimization/HILOptimizer.cpp
    LSC/LSC.cpp
    LSC/Optimization/LSCOptimizer.cpp
    LSC/Backend/LSCMachineCode.cpp
    CodeGen/CodeGen.cpp
    Diagnostics/DiagnosticEngine.cpp
    Support/SourceManager.cpp
    MCM/MCM.cpp
)

target_include_directories(HyperLangCompiler PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperLangCompiler PUBLIC cxx_std_20)

target_compile_options(HyperLangCompiler PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic>
)
