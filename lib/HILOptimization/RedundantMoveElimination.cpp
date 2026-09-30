#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runRedundantMoveElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 0; i < list.size(); ++i) {
            auto ops = operandsOf(list[i]);
            if (list[i].opcode == Opcode::LoadVariable && definesValue(list[i]) &&
                ops.size() == 1 && ops[0] == list[i].result) {
                list[i].opcode = Opcode::LoadVariable;
                setOperands(list[i], {});
                changed = true;
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
