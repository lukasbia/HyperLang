#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runInstructionCombining(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        for (std::size_t i = 0; i + 1 < list.size(); ++i) {
            if (list[i].opcode == Opcode::LoadLiteral &&
                list[i + 1].opcode == Opcode::Add) {
                const auto first = operandsOf(list[i]);
                const auto second = operandsOf(list[i + 1]);
                if (first.size() == 1 && second.size() == 2 && second[0] == "0") {
                    list[i + 1].opcode = Opcode::LoadLiteral;
                    setOperands(list[i + 1], {first[0]});
                    list.erase(list.begin() + static_cast<std::ptrdiff_t>(i));
                    changed = true;
                }
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
