#include "hyper/LLCVMBackend/Backend.h"
#include <iomanip>
#include <sstream>
#include <string>

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

TargetArchitecture parseTarget(std::string_view spelling) noexcept {
 if(spelling=="x86")return TargetArchitecture::X86;
 if(spelling=="x86_64"||spelling=="amd64")return TargetArchitecture::X86_64;
 if(spelling=="arm"||spelling=="arm32")return TargetArchitecture::ARM32;
 if(spelling=="arm64"||spelling=="aarch64")return TargetArchitecture::ARM64;
 if(spelling=="riscv32")return TargetArchitecture::RISC_V32;
 if(spelling=="riscv64")return TargetArchitecture::RISC_V64;
 if(spelling=="wasm"||spelling=="wasm32")return TargetArchitecture::WebAssembly;
 return TargetArchitecture::X86_64;
}
const char* targetName(TargetArchitecture t) noexcept {
 switch(t){case TargetArchitecture::X86:return "x86";case TargetArchitecture::X86_64:return "x86_64";case TargetArchitecture::ARM32:return "arm32";case TargetArchitecture::ARM64:return "arm64";case TargetArchitecture::RISC_V32:return "riscv32";case TargetArchitecture::RISC_V64:return "riscv64";case TargetArchitecture::WebAssembly:return "wasm";}
 return "x86_64";
}
std::string emitHex(const MachineCode& code) {
 std::ostringstream out;
 for(std::size_t i=0;i<code.bytes.size();++i){if(i)out<<' ';out<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast<unsigned>(code.bytes[i]);}
 return out.str();
}

} // namespace hyper::llcvm::backend
