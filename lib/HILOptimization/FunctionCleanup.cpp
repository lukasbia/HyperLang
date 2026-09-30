#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runFunctionCleanup(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        if (list.empty()) {
            list.push_back({Opcode::FunctionBegin, function.name});
            list.push_back({Opcode::FunctionEnd, function.name});
            changed = true;
            continue;
        }
        if (list.front().opcode != Opcode::FunctionBegin) {
            list.insert(list.begin(), Instruction{Opcode::FunctionBegin, function.name});
            changed = true;
        }
        if (list.back().opcode != Opcode::FunctionEnd) {
            list.push_back({Opcode::FunctionEnd, function.name});
            changed = true;
        }
        while (list.size() > 2 && list[0].opcode == Opcode::FunctionBegin &&
               list[1].opcode == Opcode::FunctionEnd && list.size() > 2) {
            list.insert(list.end() - 1, {Opcode::LoadLiteral, "0"});
            changed = true;
        }
    }
    return changed;
}

} // namespace hyperlang::hil
