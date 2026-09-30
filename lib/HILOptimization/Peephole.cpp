#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runPeephole(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        std::vector<Instruction> out;
        out.reserve(list.size());

        for (std::size_t i = 0; i < list.size(); ++i) {
            if (i + 1 < list.size() &&
                list[i].opcode == Opcode::Branch &&
                list[i + 1].opcode == Opcode::Branch) {
                const auto first = operandsOf(list[i]);
                const auto second = operandsOf(list[i + 1]);
                if (first.size() == 1 && second.size() == 1 && first[0] == second[0]) {
                    out.push_back(std::move(list[i + 1]));
                    ++i;
                    changed = true;
                    continue;
                }
            }
            if (list[i].opcode == Opcode::LoadLiteral && operandsOf(list[i]).empty()) {
                changed = true;
                continue;
            }
            out.push_back(std::move(list[i]));
        }

        list = std::move(out);
    }
    return changed;
}

} // namespace hyperlang::hil::optimization
