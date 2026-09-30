#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil::optimization {

bool runPeephole(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        auto& list = function.instructions;
        std::vector<Instruction> out;
        out.reserve(list.size());
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (i + 1 < list.size() && list[i].opcode == Opcode::LoadVariable &&
                list[i + 1].opcode == Opcode::StoreVariable) {
                const auto a = operandsOf(list[i]);
                const auto b = operandsOf(list[i + 1]);
                if (a.size() == 1 && b.size() == 1 && a[0] == b[0] && list[i].result.empty()) {
                    changed = true;
                    ++i;
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
