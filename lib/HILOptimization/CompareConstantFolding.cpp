#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

bool runCompareConstantFolding(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::Compare) continue;
            const auto ops = optimization::operandsOf(instruction);
            if (ops.size() < 2) continue;

            long long lhs = 0;
            long long rhs = 0;
            if (!optimization::parseInteger(ops[0], lhs) || !optimization::parseInteger(ops[1], rhs)) continue;

            bool result = lhs == rhs;
            if (ops.size() >= 3) {
                if (ops[2] == "ne") result = lhs != rhs;
                else if (ops[2] == "lt") result = lhs < rhs;
                else if (ops[2] == "le") result = lhs <= rhs;
                else if (ops[2] == "gt") result = lhs > rhs;
                else if (ops[2] == "ge") result = lhs >= rhs;
            }

            instruction.opcode = Opcode::LoadLiteral;
            optimization::setOperands(instruction, {result ? "1" : "0"});
            changed = true;
        }
    }
    return changed;
}

} // namespace hyperlang::hil
