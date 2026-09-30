#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeGenerics(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (!instruction.hasResult()) continue;
                const auto& type = instruction.result().type;
                const auto open = type.find('<');
                const auto close = type.find('>');
                if (open != std::string::npos && (close == std::string::npos || close <= open)) return false;
            }
        }
    }
    return true;
}

} // namespace hyper::hil
