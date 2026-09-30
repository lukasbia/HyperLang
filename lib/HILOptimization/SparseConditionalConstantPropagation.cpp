#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil {

bool runSparseConditionalConstantPropagation(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> known;
        for (auto& instruction : function.instructions) {
            auto ops = optimization::operandsOf(instruction);
            for (auto& op : ops) {
                const auto it = known.find(op);
                if (it != known.end()) { op = it->second; changed = true; }
            }
            optimization::setOperands(instruction, ops);

            if (instruction.opcode == Opcode::LoadLiteral && !instruction.result.empty()) {
                const auto literal = optimization::operandsOf(instruction);
                if (literal.size() == 1) known[instruction.result] = literal.front();
            } else if (optimization::usesSideEffects(instruction.opcode)) {
                if (instruction.opcode == Opcode::BranchIf) {
                    const auto branchOps = optimization::operandsOf(instruction);
                    if (!branchOps.empty()) {
                        long long value = 0;
                        if (optimization::parseInteger(branchOps.front(), value)) changed = true;
                    }
                }
                if (instruction.opcode == Opcode::Call || instruction.opcode == Opcode::StoreVariable)
                    known.clear();
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
