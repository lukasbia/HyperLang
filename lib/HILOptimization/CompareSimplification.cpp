#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runCompareSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::Compare) continue;
            auto ops = operandsOf(instruction);
            if (ops.size() < 2) continue;
            const std::string predicate = ops.size() >= 3 ? ops[2] : "eq";
            if (ops[0] == ops[1]) {
                instruction.opcode = Opcode::LoadLiteral;
                setOperands(instruction, {predicate == "ne" ? "0" : "1"});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
