#pragma once

#include "hyper/LLCVM/LLCVM.h"
#include <cstdint>
#include <string>
#include <vector>
#include <string_view>

namespace hyper::llcvm::backend {

enum class TargetArchitecture : std::uint8_t {
    X86,
    X86_64,
    ARM32,
    ARM64,
    RISC_V32,
    RISC_V64,
    WebAssembly
};

struct MachineCode {
    std::vector<std::uint8_t> bytes;
    TargetArchitecture target = TargetArchitecture::X86_64;
};

class Backend {
public:
    explicit Backend(TargetArchitecture target);
    MachineCode emit(const Module& module) const;
private:
    TargetArchitecture target_;
};

TargetArchitecture parseTarget(std::string_view spelling) noexcept;
const char* targetName(TargetArchitecture target) noexcept;
std::string emitHex(const MachineCode& code);

} // namespace hyper::llcvm::backend
