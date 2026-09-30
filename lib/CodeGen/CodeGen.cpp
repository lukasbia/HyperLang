#include "hyperlang/CodeGen/CodeGen.h"
#include <iostream>
namespace hyperlang::codegen {
void emit(const lsc::MachineCode& machine) {
    for (const auto& instruction : machine.instructions) {
        std::cout << instruction.opcode;
        if (!instruction.operand.empty()) std::cout << ' ' << instruction.operand;
        std::cout << '\n';
    }
}
}
