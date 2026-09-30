#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

bool runLiteralCanonicalization(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::LoadLiteral) continue;
            auto ops = optimization::operandsOf(instruction);
            if (ops.size() != 1) continue;
            long long value = 0;
            if (!optimization::parseInteger(ops[0], value)) continue;
            const std::string canonical = std::to_string(value);
            if (canonical != ops[0]) {
                optimization::setOperands(instruction, {canonical});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
