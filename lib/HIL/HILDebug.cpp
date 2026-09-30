#include "hyper/HIL/HIL.h"
#include <string>

namespace hyper::hil {

bool analyzeDebugMetadata(const Module& module) noexcept {
    if (module.name().empty()) return false;
    for (const auto& function : module.functions()) {
        if (function.name().empty()) return false;
        for (const auto& block : function.blocks()) {
            if (block.name().empty()) return false;
        }
    }
    return true;
}

} // namespace hyper::hil
