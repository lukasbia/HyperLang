#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runLoadStoreSimplification(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 1; i < list.size(); ++i) {
            if (list[i].opcode != Opcode::StoreVariable || list[i - 1].opcode != Opcode::StoreVariable) continue;
            const auto prev = operandsOf(list[i - 1]);
            const auto curr = operandsOf(list[i]);
            if (prev.size() == 2 && curr.size() == 2 && prev[0] == curr[0]) {
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(i - 1));
                --i;
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
