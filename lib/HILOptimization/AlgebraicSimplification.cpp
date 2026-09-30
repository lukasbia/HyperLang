#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runAlgebraicSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            auto ops = operandsOf(instruction);
            if (ops.size() != 2) continue;

            auto replaceWithIdentity = [&](const std::string& value) {
                long long literal = 0;
                instruction.opcode = parseInteger(value, literal) ? Opcode::LoadLiteral : Opcode::LoadVariable;
                setOperands(instruction, {value});
                changed = true;
            };

            if (instruction.opcode == Opcode::Add || instruction.opcode == Opcode::Subtract) {
                if (ops[1] == "0") replaceWithIdentity(ops[0]);
                else if (instruction.opcode == Opcode::Add && ops[0] == "0") replaceWithIdentity(ops[1]);
            } else if (instruction.opcode == Opcode::Multiply) {
                if (ops[1] == "1") replaceWithIdentity(ops[0]);
                else if (ops[0] == "1") replaceWithIdentity(ops[1]);
                else if (ops[1] == "0" || ops[0] == "0") {
                    instruction.opcode = Opcode::LoadLiteral;
                    setOperands(instruction, {"0"});
                    changed = true;
                }
            } else if (instruction.opcode == Opcode::Divide && ops[1] == "1") {
                replaceWithIdentity(ops[0]);
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
