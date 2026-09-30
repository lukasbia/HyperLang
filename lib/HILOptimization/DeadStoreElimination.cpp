#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <algorithm>

namespace hyperlang::hil {

bool runDeadStoreElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 0; i + 1 < list.size();) {
            if (list[i].opcode == Opcode::StoreVariable &&
                list[i + 1].opcode == Opcode::StoreVariable) {
                const auto a = optimization::operandsOf(list[i]);
                const auto b = optimization::operandsOf(list[i + 1]);
                if (a.size() == 2 && b.size() == 2 && a[0] == b[0]) {
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
