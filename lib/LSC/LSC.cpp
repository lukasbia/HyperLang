#include "hyperlang/LSC/LSC.h"
#include "hyperlang/HIL/HIL.h"
#include <iostream>
#include <utility>
namespace hyperlang::lsc {
void LiveCompiler::synchronize(const hil::Module& module) {
    snapshot_.module = module;
    ++snapshot_.revision;
    if (callback_) callback_(snapshot_);
}
void LiveCompiler::setUpdateCallback(UpdateCallback callback) {
    callback_ = std::move(callback);
}
const CompilationSnapshot& LiveCompiler::snapshot() const {
    return snapshot_;
}
IR lower(const hil::Module& module) {
    IR ir;
    for (const auto& function : module.functions) {
        for (const auto& instruction : function.instructions) {
            ir.instructions.push_back({opcodeName(instruction.opcode), instruction.operand});
        }
    }
    return ir;
}
void emit(const MachineCode& machine) {
    for (const auto& instruction : machine.instructions) {
        std::cout << instruction.opcode;
        if (!instruction.operand.empty()) std::cout << ' ' << instruction.operand;
        std::cout << '\n';
    }
}
}
