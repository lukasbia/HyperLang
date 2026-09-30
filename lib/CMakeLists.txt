add_library(HyperLangCompiler
    Driver/Driver.cpp
    Lexer/Lexer.cpp
    Lexer/Token.cpp
    Parser/Parser.cpp
    Sema/Sema.cpp
    HIL/HIL.cpp
    HIL/Optimization/HILOptimizer.cpp
    HILOptimization/OptimizationUtils.cpp
    HILOptimization/ConstantFolding.cpp
    HILOptimization/ConstantPropagation.cpp
    HILOptimization/CopyPropagation.cpp
    HILOptimization/AlgebraicSimplification.cpp
    HILOptimization/CommonSubexpressionElimination.cpp
    HILOptimization/DeadCodeElimination.cpp
    HILOptimization/BranchSimplification.cpp
    HILOptimization/Peephole.cpp
    HILOptimization/LoadStoreSimplification.cpp
    HILOptimization/StrengthReduction.cpp
    HILOptimization/CompareSimplification.cpp
    HILOptimization/InstructionCombining.cpp
    HILOptimization/RedundantMoveElimination.cpp
    HILOptimization/FunctionCleanup.cpp
    LSC/LSC.cpp
    LSC/Optimization/LSCOptimizer.cpp
    LSC/Backend/LSCMachineCode.cpp
    CodeGen/CodeGen.cpp
    MCM/MCM.cpp
    Diagnostics/DiagnosticEngine.cpp
    Support/SourceManager.cpp
)

target_include_directories(HyperLangCompiler PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(HyperLangCompiler PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(HyperLangCompiler PRIVATE /W4 /permissive- /O2)
else()
    target_compile_options(HyperLangCompiler PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()