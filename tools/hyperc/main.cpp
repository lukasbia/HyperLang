#include "hyper/Compiler/Compiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void printLexer(const std::vector<hyper::LexerDiagnostic>& diagnostics) {
    for (const auto& diagnostic : diagnostics) {
        std::cerr << diagnostic.location.line << ":"
                  << diagnostic.location.column << ": "
                  << diagnostic.message << "\n";
    }
}

void printParser(const std::vector<hyper::ParseDiagnostic>& diagnostics) {
    for (const auto& diagnostic : diagnostics) {
        std::cerr << diagnostic.location.line << ":"
                  << diagnostic.location.column << ": "
                  << diagnostic.message << "\n";
    }
}

void printSema(const std::vector<hyper::sema::Diagnostic>& diagnostics) {
    for (const auto& diagnostic : diagnostics) {
        std::cerr << diagnostic.message << "\n";
    }
}

}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: hyperc <file.hl> [--target=<target>]\n";
        return 2;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "hyperc: unable to open '" << argv[1] << "'\n";
        return 1;
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();

    hyper::compiler::CompilerOptions options;
    options.sourceFile = argv[1];

    for (int i = 2; i < argc; ++i) {
        const std::string argument = argv[i];
        using Target = hyper::llcvm::backend::TargetArchitecture;
        if (argument == "--target=x86") options.target = Target::X86;
        else if (argument == "--target=x86_64") options.target = Target::X86_64;
        else if (argument == "--target=arm32") options.target = Target::ARM32;
        else if (argument == "--target=arm64") options.target = Target::ARM64;
        else if (argument == "--target=riscv32") options.target = Target::RISC_V32;
        else if (argument == "--target=riscv64") options.target = Target::RISC_V64;
        else if (argument == "--target=wasm") options.target = Target::WebAssembly;
        else if (argument == "--no-machine-code") options.emitMachineCode = false;
    }

    hyper::compiler::Compiler compiler(options);
    const auto result = compiler.compile(buffer.str());

    printLexer(result.lexerDiagnostics);
    printParser(result.parserDiagnostics);
    printSema(result.semaDiagnostics);

    if (!result.success) return 1;

    std::cout << "HyperLang compilation succeeded\n"
              << "HIL functions: " << result.hilModule.functions().size() << "\n"
              << "LSC lines: " << result.lscModule.lines().size() << "\n";

    if (options.emitMachineCode)
        std::cout << "machine bytes: " << result.machineCode.bytes.size() << "\n";

    return 0;
}
