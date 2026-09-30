#include "hyperlang/Driver/Driver.h"
#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Parser/Parser.h"
#include "hyperlang/Sema/Sema.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/LSC/LSC.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/CodeGen/CodeGen.h"
#include <fstream>
#include <sstream>

namespace hyperlang::driver {
int Driver::run(const std::string& inputPath, const DriverOptions& options) {
    std::ifstream file(inputPath, std::ios::binary);
    if (!file) return 1;
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return compileSource(buffer.str(), options);
}

int Driver::compileSource(const std::string& source, const DriverOptions& options) {
    lexer::Lexer lexer(source);
    auto tokens = lexer.tokenize();

    parser::Parser parser(std::move(tokens));
    auto tree = parser.parse();
    if (!tree) return 1;

    sema::Analyzer sema;
    if (!sema.analyze(*tree)) return 1;

    hil::Module module = hil::lower(*tree);
    if (options.optimize) hil::optimize(module);

    mcm::insertOwnershipOperations(module);

    lsc::LiveCompiler liveCompiler;
    liveCompiler.synchronize(module);

    lsc::IR ir = lsc::lower(module);
    if (options.optimize) lsc::optimize(ir);

    lsc::MachineCode machine = lsc::generateMachineCode(ir);
    codegen::emit(machine);
    return 0;
}
}
