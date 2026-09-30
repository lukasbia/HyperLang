#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runBranchSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::BranchIf) continue;
            auto ops = operandsOf(instruction);
            if (ops.size() < 2) continue;
            long long condition = 0;
            if (!parseInteger(ops[0], condition)) continue;
            instruction.opcode = Opcode::Branch;
            if (condition != 0) {
                setOperands(instruction, {ops[1]});
            } else if (ops.size() >= 3) {
                setOperands(instruction, {ops[2]});
            } else {
                instruction.opcode = Opcode::FunctionEnd;
                setOperands(instruction, {});
            }
            changed = true;
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
