#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <cstddef>

namespace hyperlang::hil {

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
            const auto opcode = list[i].opcode;
            if (opcode != Opcode::Return && opcode != Opcode::Branch && opcode != Opcode::BranchIf) continue;

            std::size_t end = i + 1;
            while (end < list.size() && list[end].opcode != Opcode::FunctionEnd) ++end;
            if (end > i + 1) {
                list.erase(list.begin() + static_cast<std::ptrdiff_t>(i + 1),
                           list.begin() + static_cast<std::ptrdiff_t>(end));
                changed = true;
            }
            break;
        }
    }

    return changed;
}

} // namespace hyperlang::hil
