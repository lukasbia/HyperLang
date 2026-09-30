#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

CFGSummary analyzeCFG(const Module& module) {
    CFGSummary summary;
    for (const auto& function : module.functions) {
        bool ended = false;
        for (const auto& instruction : function.instructions) {
            if (instruction.opcode == Opcode::Branch) ++summary.branchCount;
            if (instruction.opcode == Opcode::BranchIf) { ++summary.branchCount; ++summary.conditionalBranchCount; }
            if (optimization::isTerminator(instruction.opcode)) {
                ++summary.terminatorCount;
                ended = true;
            } else {
                ended = false;
            }
        }
        if (!function.instructions.empty() && !ended) summary.structurallyValid = false;
    }
    return summary;
}

} // namespace hyperlang::hil
