#include "hyper/HIL/HIL.h"

namespace hyper::hil {

bool analyzeActors(const Module& module) noexcept {
    for (const auto& function : module.functions()) {
        if (function.name().find("actor") != std::string::npos || function.name().find("Actor") != std::string::npos) {
            return !function.blocks().empty();
        }
    }
    return true;
}

} // namespace hyper::hil
