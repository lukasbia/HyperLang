#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeAsync(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        if (function.name().find("async") != std::string::npos || function.name().find("Async") != std::string::npos) {
            return !function.blocks().empty();
        }
    }
    return true;
}

} // namespace hyper::hil
