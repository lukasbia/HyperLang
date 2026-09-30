#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeClosures(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (instruction.opcode() == Opcode::Call && instruction.operands().empty()) return false;
            }
        }
    }
    return true;
}

} // namespace hyper::hil
