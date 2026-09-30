#include "hyper/LLCVMBackend/Backend.h"

namespace hyper::llcvm::backend {

Backend::Backend(TargetArchitecture target) : target_(target) {}

MachineCode Backend::emit(const Module& module) const {
    MachineCode result;
    result.target = target_;
    for (const auto& line : module.lines()) {
        for (const auto& instruction : line.instructions) {
            result.bytes.push_back(static_cast<std::uint8_t>(instruction.opcode));
        }
    }
    return result;
}

} // namespace hyper::llcvm::backend
