#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeEnums(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (instruction.hasResult() && instruction.result().type.find("enum") != std::string::npos && instruction.result().name.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace hyper::hil
