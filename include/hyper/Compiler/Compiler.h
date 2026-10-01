#pragma once

#include "hyper/HIL/HIL.h"
#include "hyper/HILOptimization/HILOptimization.h"
#include "hyper/LLCVM/IRGenerator.h"
#include "hyper/LLCVM/LLCVM.h"
#include "hyper/LLCVMBackend/Backend.h"
#include "hyper/Parse/Lexer.h"
#include "hyper/Parse/Parser.h"
#include "hyper/Sema/Sema.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace hyper::compiler {

struct CompilerOptions {
    std::string sourceFile;
    std::string moduleName = "HyperModule";
    llcvm::backend::TargetArchitecture target =
        llcvm::backend::TargetArchitecture::X86_64;
    bool emitMachineCode = true;
};

struct CompilationResult {
    bool success = false;
    std::vector<LexerDiagnostic> lexerDiagnostics;
    std::vector<ParseDiagnostic> parserDiagnostics;
    std::vector<sema::Diagnostic> semaDiagnostics;
    std::unique_ptr<SyntaxNode> syntaxTree;
    hil::Module hilModule{"HyperModule"};
    llcvm::Module lscModule{"HyperModule"};
    llcvm::backend::MachineCode machineCode{};
};

class Compiler {
public:
    explicit Compiler(CompilerOptions options = {});
    CompilationResult compile(std::string_view source);

private:
    CompilerOptions options_;
    static void lowerSyntaxToHIL(const SyntaxNode&, hil::Module&);
    static void appendLineIR(std::string_view, llcvm::Module&);
    static bool runSemanticChecks(const SyntaxNode&, sema::Sema&);
};

} // namespace hyper::compiler
