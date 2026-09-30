#include "hyperlang/LSC/Backend/LSCMachineCode.h"
namespace hyperlang::lsc {
MachineCode generateMachineCode(const IR& ir) {
    MachineCode machine;
    machine.instructions.reserve(ir.instructions.size());
    for (const auto& instruction : ir.instructions) {
        machine.instructions.push_back({instruction.opcode, instruction.operand});
    }
    return machine;
}
}
