#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_set>

namespace hyperlang::hil::optimization {

bool runDeadCodeElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_set<std::string> used;
        for (const auto& instruction : function.instructions) {
            for (const auto& op : operandsOf(instruction)) used.insert(op);
        }

        auto& list = function.instructions;
        list.erase(std::remove_if(list.begin(), list.end(), [&](const Instruction& instruction) {
            if (!definesValue(instruction) || !isPure(instruction.opcode)) return false;
            if (used.count(instruction.result)) return false;
            changed = true;
            return true;
        }), list.end());
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
