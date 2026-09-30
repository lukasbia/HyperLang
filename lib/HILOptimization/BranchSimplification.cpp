#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runBranchSimplification(Module& module) {
    bool changed = false;

    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (list[i].opcode != Opcode::BranchIf) continue;

            const auto ops = operandsOf(list[i]);
            if (ops.size() < 2) continue;

            long long condition = 0;
            if (!parseInteger(ops[0], condition)) continue;

            if (condition != 0) {
                list[i].opcode = Opcode::Branch;
                setOperands(list[i], {ops[1]});
                changed = true;
            } else if (ops.size() >= 3) {
                list[i].opcode = Opcode::Branch;
                setOperands(list[i], {ops[2]});
                changed = true;
            } else {
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(i));
                --i;
                changed = true;
            }
        }
    }

    return changed;
}

} // namespace hyperlang::hil::optimization
