#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeHILParserOutput(const Module& module) noexcept {
    if (module.name().empty()) return false;
    for (const auto& function : module.functions()) {
        if (function.name().empty() || function.blocks().empty()) return false;
        for (const auto& block : function.blocks()) {
            if (block.name().empty()) return false;
            for (const auto& instruction : block.instructions()) {
                if (instruction.opcode() == Opcode::Nop && instruction.hasResult()) return false;
            }
        }
    }
    return true;
}

} // namespace hyper::hil
