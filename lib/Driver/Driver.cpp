#include "hyperlang/Driver/Driver.h"
#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Parser/Parser.h"
#include "hyperlang/Sema/Sema.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/LSC/LSC.h"
#include "hyperlang/MCM/MCM.h"
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
    parser::Parser parser(tokens);
    auto tree = parser.parse();
    sema::Sema sema;
    if (!sema.check(tree)) return 1;
    hil::Module module = hil::lower(tree);
    if (options.optimize) hil::optimize(module);
    mcm::insertOwnershipOperations(module);
    lsc::IR ir = lsc::lower(module);
    if (options.optimize) lsc::optimize(ir);
    lsc::MachineCode machine = lsc::generateMachineCode(ir);
    lsc::emit(machine);
    return 0;
}
}
