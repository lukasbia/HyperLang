#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runCompareSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::Compare) continue;

            const auto ops = operandsOf(instruction);
            if (ops.size() < 2 || ops[0] != ops[1]) continue;

            const std::string predicate = ops.size() >= 3 ? ops[2] : "eq";
            if (predicate == "eq" || predicate == "ne") {
                instruction.opcode = Opcode::LoadLiteral;
                setOperands(instruction, {predicate == "eq" ? "1" : "0"});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
