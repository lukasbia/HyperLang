#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include <algorithm>
#include <utility>
namespace hyperlang::hil {
void optimize(Module& module) {
    for (auto& function : module.functions) {
        auto& instructions = function.instructions;
        instructions.erase(std::remove_if(instructions.begin(), instructions.end(), [](const Instruction& instruction) {
            return instruction.opcode == Opcode::LoadLiteral && instruction.operand.empty();
        }), instructions.end());
        if (instructions.empty()) {
            instructions.push_back({Opcode::FunctionBegin, function.name});
            instructions.push_back({Opcode::FunctionEnd, function.name});
        }
    }
}
}
