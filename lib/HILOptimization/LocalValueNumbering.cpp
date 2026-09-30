#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil {

bool runLocalValueNumbering(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> values;
        for (auto& instruction : function.instructions) {
            if (!optimization::isPure(instruction.opcode) || instruction.result.empty()) {
                if (optimization::usesSideEffects(instruction.opcode)) values.clear();
                continue;
            }

            const auto ops = optimization::operandsOf(instruction);
            std::string key = opcodeName(instruction.opcode);
            for (const auto& op : ops) key += "|" + op;

            const auto it = values.find(key);
            if (it != values.end() && it->second != instruction.result) {
                instruction.opcode = Opcode::LoadVariable;
                optimization::setOperands(instruction, {it->second});
                changed = true;
            } else {
                values.emplace(std::move(key), instruction.result);
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
