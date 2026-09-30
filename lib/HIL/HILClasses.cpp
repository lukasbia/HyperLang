#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeClasses(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        for (const auto& block : function.blocks()) {
            for (const auto& instruction : block.instructions()) {
                if (instruction.hasResult() && instruction.result().type.find("class") != std::string::npos) {
                    if (instruction.result().name.empty()) return false;
                }
            }
        }
    }
    return true;
}

} // namespace hyper::hil
