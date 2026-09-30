#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runAlgebraicSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            auto ops = operandsOf(instruction);
            if (ops.size() != 2) continue;
            if (instruction.opcode == Opcode::Add || instruction.opcode == Opcode::Subtract) {
                if (ops[1] == "0") { instruction.opcode = Opcode::LoadVariable; setOperands(instruction, {ops[0]}); changed = true; }
                else if (instruction.opcode == Opcode::Add && ops[0] == "0") { instruction.opcode = Opcode::LoadVariable; setOperands(instruction, {ops[1]}); changed = true; }
            } else if (instruction.opcode == Opcode::Multiply) {
                if (ops[1] == "1" || ops[0] == "1") {
                    instruction.opcode = Opcode::LoadVariable;
                    setOperands(instruction, {ops[1] == "1" ? ops[0] : ops[1]});
                    changed = true;
                } else if (ops[1] == "0" || ops[0] == "0") {
                    instruction.opcode = Opcode::LoadLiteral;
                    setOperands(instruction, {"0"});
                    changed = true;
                }
            } else if (instruction.opcode == Opcode::Divide && ops[1] == "1") {
                instruction.opcode = Opcode::LoadVariable;
                setOperands(instruction, {ops[0]});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
