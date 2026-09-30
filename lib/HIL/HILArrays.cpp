#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeArrays(const Module& module) noexcept {
    bool sawArrayOperation = false;
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (instruction.opcode() == Opcode::Load || instruction.opcode() == Opcode::Store) {
                    sawArrayOperation = true;
                    if (instruction.operands().empty()) return false;
                }
            }
        }
    }
    return sawArrayOperation || instructionCount(module) == 0;
}

} // namespace hyper::hil
