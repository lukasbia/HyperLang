#include "hyperlang/Driver/Driver.h"
#include "hyperlang/CodeGen/CodeGen.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/LSC/LSC.h"
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
    // Source code -> Lexer
    lexer::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    // Lexer -> Parser
    parser::Parser parser(std::move(tokens));
    auto tree = parser.parse();
    if (!tree) return 1;

    // Parser -> Sema
    sema::Analyzer sema;
    if (!sema.analyze(*tree)) return 1;

    // Sema -> Hyper Intermediate Language (HIL)
    hil::Module hilModule = hil::lower(*tree);

    // HIL optimization
    if (options.optimize) {
        hil::optimize(hilModule);
    }

    if (options.emitHIL) {
        for (const auto &function : hilModule.functions) {
            std::cout << "hil.function " << function.name << "\n";
            for (const auto &instruction : function.instructions) {
                std::cout << "  " << hil::opcodeName(instruction.opcode);
                if (!instruction.operand.empty()) {
                    std::cout << ' ' << instruction.operand;
                }
                std::cout << '\n';
            }
        }
        return 0;
    }

    // HIL -> Live Synchronized Compiler IR (LSC IR)
    lsc::LiveCompiler liveCompiler;
    liveCompiler.synchronize(hilModule);
    lsc::IR lscIR = lsc::lower(hilModule);

    // LSC optimization
    if (options.optimize) {
        lsc::optimize(lscIR);
    }

    if (options.emitLSCIR) {
        std::cout << "lsc.revision " << lscIR.revision << "\n";
        for (const auto &instruction : lscIR.instructions) {
            std::cout << instruction.opcode;
            if (!instruction.operand.empty()) {
                std::cout << ' ' << instruction.operand;
            }
            std::cout << '\n';
        }
        return 0;
    }

    // LSC IR -> machine code and backend
    lsc::MachineCode machineCode = lsc::generateMachineCode(lscIR);

    // Optimizer / cleanup are represented by the LSC optimization pass and
    // the backend's final instruction cleanup before CodeGen.
    // CodeGen -> linker boundary
    codegen::emit(machineCode);

    return 0;
}

} // namespace hyperlang::driver
