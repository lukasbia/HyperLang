#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <algorithm>

namespace hyperlang::hil::optimization {

bool runRedundantMoveElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        list.erase(std::remove_if(list.begin(), list.end(), [&](const Instruction& instruction) {
            if (instruction.opcode != Opcode::LoadVariable || !definesValue(instruction)) return false;
            const auto ops = operandsOf(instruction);
            if (ops.size() != 1 || ops.front() != instruction.result) return false;
            changed = true;
            return true;
        }), list.end());
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
