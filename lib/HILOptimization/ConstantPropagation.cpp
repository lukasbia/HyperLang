#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil::optimization {

bool runConstantPropagation(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> constants;
        for (auto& instruction : function.instructions) {
            auto ops = operandsOf(instruction);
            for (auto& op : ops) {
                auto it = constants.find(op);
                if (it != constants.end()) { op = it->second; changed = true; }
            }
            setOperands(instruction, ops);
            if (instruction.opcode == Opcode::LoadLiteral && definesValue(instruction)) {
                auto literal = operandsOf(instruction);
                if (literal.size() == 1 && !literal[0].empty()) constants[instruction.result] = literal[0];
            } else if (instruction.hasSideEffects || usesSideEffects(instruction.opcode)) {
                if (instruction.opcode == Opcode::StoreVariable && !ops.empty()) {
                    constants.erase(ops.front());
                }
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
