#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeExistentials(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (!instruction.hasResult()) continue;
                const auto& type = instruction.result().type;
                if ((type.find("any") != std::string::npos || type.find("existential") != std::string::npos) && instruction.result().name.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace hyper::hil
