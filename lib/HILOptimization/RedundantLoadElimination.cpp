#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

bool runRedundantLoadElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 1; i < list.size(); ++i) {
            if (list[i].opcode != Opcode::LoadVariable || !list[i].result.empty()) continue;
            const auto current = optimization::operandsOf(list[i]);
            if (current.size() != 1) continue;
            const auto previous = optimization::operandsOf(list[i - 1]);
            if (list[i - 1].opcode == Opcode::LoadVariable && previous == current &&
                list[i - 1].result.empty()) {
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(i));
                changed = true;
                --i;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
