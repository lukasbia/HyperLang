#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runStrengthReduction(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            auto ops = operandsOf(instruction);
            if (instruction.opcode != Opcode::Multiply || ops.size() != 2) continue;
            const std::string& value = ops[0];
            if (ops[1] == "2") {
                instruction.opcode = Opcode::Add;
                setOperands(instruction, {value, value});
                changed = true;
            } else if (ops[0] == "2") {
                instruction.opcode = Opcode::Add;
                setOperands(instruction, {ops[1], ops[1]});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
