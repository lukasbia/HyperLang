#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil::optimization {

bool runCommonSubexpressionElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> available;
        for (auto& instruction : function.instructions) {
            if (!isPure(instruction.opcode) || !definesValue(instruction)) {
                if (usesSideEffects(instruction.opcode)) available.clear();
                continue;
            }
            const auto ops = operandsOf(instruction);
            std::string key = opcodeName(instruction.opcode);
            for (const auto& op : ops) key += "|" + op;
            auto it = available.find(key);
            if (it != available.end() && it->second != instruction.result) {
                instruction.opcode = Opcode::LoadVariable;
                setOperands(instruction, {it->second});
                changed = true;
            } else {
                available[key] = instruction.result;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
