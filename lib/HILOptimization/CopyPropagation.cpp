#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil::optimization {

bool runCopyPropagation(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> copies;
        for (auto& instruction : function.instructions) {
            auto ops = operandsOf(instruction);
            for (auto& op : ops) {
                auto it = copies.find(op);
                if (it != copies.end()) { op = it->second; changed = true; }
            }
            setOperands(instruction, ops);
            if (instruction.opcode == Opcode::LoadVariable && definesValue(instruction) && ops.size() == 1)
                copies[instruction.result] = ops[0];
            else if (instruction.result.empty())
                for (const auto& op : ops) copies.erase(op);
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
