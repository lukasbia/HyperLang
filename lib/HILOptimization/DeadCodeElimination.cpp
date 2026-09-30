#include "hyper/HIL/HIL.h"
#include "hyper/HILOptimization/HILOptimization.h"
#include <unordered_set>

namespace hyper::hil::optimization {

bool runDeadCodeElimination(Module& module) {
    bool changed = false;
    for (auto& function : module.functions()) {
        std::unordered_set<std::uint32_t> used;
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                for (const auto& operand : instruction.operands()) used.insert(operand.id);
            }
        }

        for (auto& block : function.blocks()) {
            auto& instructions = block.instructions();
            for (std::size_t i = 0; i < instructions.size();) {
                const auto& instruction = instructions[i];
                if (instruction.hasResult() && isPureOperation(instruction.opcode()) &&
                    used.find(instruction.result().id) == used.end()) {
                    block.eraseInstruction(i);
                    changed = true;
                    continue;
                }
                ++i;
            }
        }
    }
    return changed;
}

} // namespace hyper::hil::optimization
