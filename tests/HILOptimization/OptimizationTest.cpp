#include "hyperlang/HIL/Optimization/HILOptimizer.h"

#include <cassert>

using namespace hyperlang::hil;

int main() {
    Module module;
    Function function;
    function.name = "main";

    Instruction folded{Opcode::Add, "40,2"};
    folded.result = "v0";
    function.instructions.push_back(folded);

    Instruction removable{Opcode::Multiply, "9,9"};
    removable.result = "unused";
    function.instructions.push_back(removable);

    function.instructions.push_back({Opcode::Return, "v0"});
    function.instructions.push_back({Opcode::FunctionEnd, "main"});
    module.functions.push_back(function);

    OptimizationOptions options;
    options.constantPropagation = false;
    options.sparseConditionalConstantPropagation = false;
    options.copyPropagation = false;

    const auto stats = optimizeModule(module, options);
    assert(stats.iterations > 0);
    assert(stats.transformations > 0);

    const auto cfg = analyzeCFG(module);
    assert(cfg.structurallyValid);

    bool hasFoldedLiteral = false;
    bool hasReturn = false;
    bool hasDeadValue = false;

    for (const auto& instruction : module.functions[0].instructions) {
        if (instruction.opcode == Opcode::LoadLiteral && instruction.operand == "42")
            hasFoldedLiteral = true;
        if (instruction.opcode == Opcode::Return && instruction.operand == "v0")
            hasReturn = true;
        if (instruction.result == "unused")
            hasDeadValue = true;
    }

    assert(hasFoldedLiteral);
    assert(hasReturn);
    assert(!hasDeadValue);
    return 0;
}
