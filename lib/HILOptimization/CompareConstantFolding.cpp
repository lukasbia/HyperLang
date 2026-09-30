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
            if (!optimization::parseInteger(ops[0], lhs) ||
                !optimization::parseInteger(ops[1], rhs)) continue;

            const std::string predicate = ops.size() >= 3 ? ops[2] : "eq";
            bool value = false;
            bool knownPredicate = true;
            if (predicate == "eq") value = lhs == rhs;
            else if (predicate == "ne") value = lhs != rhs;
            else if (predicate == "lt") value = lhs < rhs;
            else if (predicate == "le") value = lhs <= rhs;
            else if (predicate == "gt") value = lhs > rhs;
            else if (predicate == "ge") value = lhs >= rhs;
            else knownPredicate = false;

            if (!knownPredicate) continue;

            instruction.opcode = Opcode::LoadLiteral;
            optimization::setOperands(instruction, {value ? "1" : "0"});
            changed = true;
        }
    }

    return changed;
}

} // namespace hyperlang::hil
