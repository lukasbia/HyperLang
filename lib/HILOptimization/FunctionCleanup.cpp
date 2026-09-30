#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <algorithm>

namespace hyperlang::hil::optimization {

bool runFunctionCleanup(Module& module) {
    bool changed = false;

    for (auto& function : module.functions) {
        auto& list = function.instructions;

        if (list.empty()) {
            list.push_back({Opcode::FunctionBegin, function.name});
            list.push_back({Opcode::FunctionEnd, function.name});
            changed = true;
            continue;
        }

        if (list.front().opcode != Opcode::FunctionBegin) {
            list.insert(list.begin(), Instruction{Opcode::FunctionBegin, function.name});
            changed = true;
        }
        if (list.back().opcode != Opcode::FunctionEnd) {
            list.push_back({Opcode::FunctionEnd, function.name});
            changed = true;
        }

        for (std::size_t i = 1; i + 1 < list.size(); ++i) {
            if (list[i].opcode != Opcode::Return && list[i].opcode != Opcode::Branch &&
                list[i].opcode != Opcode::BranchIf) continue;
            auto end = std::find(list.begin() + static_cast<std::ptrdiff_t>(i + 1), list.end(),
                                 Instruction{Opcode::FunctionEnd, function.name});
            if (end == list.end()) break;
            if (end != list.begin() + static_cast<std::ptrdiff_t>(i + 1)) {
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(i + 1), end);
                changed = true;
            }
            break;
        }
    }

    return changed;
}

} // namespace hyperlang::hil::optimization
