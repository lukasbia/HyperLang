#include "hyperlang/HIL/Optimization/HILOptimizer.h"

#include <cassert>
#include <string>

using namespace hyperlang::hil;

int main() {
    Module module;
    Function function;
    function.name = "main";

    Instruction folded{Opcode::Add, "40,2"};
    folded.result = "v0";
    function.instructions.push_back(folded);

    Instruction constant{Opcode::LoadLiteral, "7"};
    constant.result = "v1";
    function.instructions.push_back(constant);

    Instruction addZero{Opcode::Add, "v1,0"};
    addZero.result = "v2";
    function.instructions.push_back(addZero);

    Instruction compare{Opcode::Compare, "5,5,eq"};
    compare.result = "v3";
    function.instructions.push_back(compare);

    Instruction branch{Opcode::BranchIf, "1,done"};
    function.instructions.push_back(branch);

    Instruction dead{Opcode::Multiply, "9,9"};
    dead.result = "unused";
    function.instructions.push_back(dead);

    function.instructions.push_back({Opcode::Return, "v2"});
    function.instructions.push_back({Opcode::FunctionEnd, "main"});
    module.functions.push_back(function);

    const auto stats = optimizeModule(module);
    assert(stats.iterations > 0);
    assert(stats.transformations > 0);

    bool hasFoldedLiteral = false;
    bool hasReturn = false;
    for (const auto& instruction : module.functions[0].instructions) {
        if (instruction.opcode == Opcode::LoadLiteral && instruction.operand == "42") hasFoldedLiteral = true;
        if (instruction.opcode == Opcode::Return) hasReturn = true;
    }

    assert(hasFoldedLiteral);
    assert(hasReturn);
    return 0;
}
