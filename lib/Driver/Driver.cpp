#include "hyperlang/Driver/Driver.h"
#include "hyperlang/CodeGen/CodeGen.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/LSC/LSC.h"
#include "hyperlang/LSC/Optimization/LSCOptimizer.h"
#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Parser/Parser.h"
#include "hyperlang/Sema/Sema.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace hyperlang::driver {

int Driver::run(const std::string &inputPath, const DriverOptions &options) {
    std::ifstream file(inputPath, std::ios::binary);
    if (!file) {
        std::cerr << "hyperlang: unable to open source file: " << inputPath << "\n";
        return 1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return compileSource(buffer.str(), options);
}

int Driver::compileSource(const std::string &source,
                          const DriverOptions &options) {
    // Source -> Lexer
    lexer::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // Lexer -> Parser
    parser::Parser parser(std::move(tokens));
    auto tree = parser.parse();
    if (!tree) return 1;

    // Parser -> Sema
    sema::Analyzer sema;
    if (!sema.analyze(*tree)) return 1;

    // Sema -> HIL
    hil::Module hilModule = hil::lower(*tree);

    // HIL optimization
    if (options.optimize) {
        hil::optimize(hilModule);
    }

    if (options.emitHIL) {
        std::cout << hil::print(hilModule);
        return 0;
    }

    // HIL -> LSC IR
    lsc::LiveCompiler liveCompiler;
    liveCompiler.synchronize(hilModule);
    lsc::IR lscIR = lsc::lower(hilModule);

    // LSC optimization
    if (options.optimize) {
        lsc::optimize(lscIR);
    }

    if (options.emitLSCIR) {
        std::cout << lsc::print(lscIR);
        return 0;
    }

    // LSC -> machine code/backend
    lsc::MachineCode machineCode = lsc::generateMachineCode(lscIR, options.target);

    if (options.emitMachineCode) {
        std::cout << machineCode.print();
        return 0;
    }

    // Optimizer -> cleanup -> codegen
    codegen::CodeGenerationResult generated = codegen::generate(machineCode);

    if (options.emitAssembly) {
        std::cout << generated.assembly;
        return 0;
    }

    if (options.emitHex) {
        std::cout << generated.hex << "\n";
        return 0;
    }

    // Linker boundary. The linker is intentionally kept behind CodeGen so
    // platform-specific linkers can be attached without changing the frontend.
    if (options.link) {
        return codegen::link(generated, options.outputPath) ? 0 : 1;
    }

    return 0;
}

} // namespace hyperlang::driver
