#include "hyperlang/HIL/Optimization/HILOptimizer.h"

namespace hyperlang::hil {

CostSummary estimateOptimizationCost(const Module& module) {
    CostSummary summary;
    for (const auto& function : module.functions) {
        for (const auto& instruction : function.instructions) {
            ++summary.instructionCount;
            switch (instruction.opcode) {
                case Opcode::Call: summary.estimatedCost += 8; break;
                case Opcode::Divide: summary.estimatedCost += 6; break;
                case Opcode::Multiply: summary.estimatedCost += 3; break;
                case Opcode::Branch:
                case Opcode::BranchIf: summary.estimatedCost += 2; break;
                case Opcode::LoadVariable:
                case Opcode::StoreVariable: summary.estimatedCost += 2; break;
                default: summary.estimatedCost += 1; break;
            }
        }
    }
    return summary;
}

} // namespace hyperlang::hil
