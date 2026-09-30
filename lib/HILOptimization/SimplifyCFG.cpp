#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

bool runSimplifyCFG(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 0; i + 1 < list.size();) {
            if (list[i].opcode == Opcode::Branch && list[i + 1].opcode == Opcode::Branch) {
                const auto first = optimization::operandsOf(list[i]);
                const auto second = optimization::operandsOf(list[i + 1]);
                if (first.size() == 1 && second.size() == 1 && first[0] == second[0]) {
                    list.erase(list.begin() + static_cast<std::ptrdiff_t>(i));
                    changed = true;
                    continue;
                }
            }
            ++i;
        }
    }
    return changed;
}

} // namespace hyperlang::hil
