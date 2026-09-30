#include "hyper/HIL/HIL.h"
#include "hyper/HILOptimization/HILOptimization.h"
#include <charconv>
#include <string>
#include <unordered_map>

namespace hyper::hil::optimization {

bool runConstantPropagation(Module& module) {
    bool changed = false;
    for (auto& function : module.functions()) {
        std::unordered_map<std::uint32_t, Value> constants;
        for (auto& block : function.blocks()) {
            for (auto& instruction : block.instructions()) {
                if (instruction.opcode() == Opcode::Constant && instruction.hasResult() && !instruction.operands().empty()) {
                    constants[instruction.result().id] = instruction.operands().front();
                    continue;
                }
                if (!isPureOperation(instruction.opcode())) continue;
                for (auto& operand : instruction.operands()) {
                    auto it = constants.find(operand.id);
                    if (it != constants.end() && operand.id != 0) {
                        operand = it->second;
                        changed = true;
                    }
                }
            }
        }
    }
    return changed;
}

} // namespace hyper::hil::optimization
