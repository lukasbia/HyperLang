#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

#include <algorithm>

namespace hyperlang::hil {

LoopSummary analyzeLoops(const Module& module) {
    LoopSummary summary;

    for (const auto& function : module.functions) {
        std::vector<std::string> branchTargets;

        for (const auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::Branch &&
                instruction.opcode != Opcode::BranchIf) continue;

            const auto ops = optimization::operandsOf(instruction);
            if (ops.empty()) continue;

            const std::string& target = ops.back();
            if (std::find(branchTargets.begin(), branchTargets.end(), target) != branchTargets.end()) {
                ++summary.branchBackedgeCandidates;
            }
            branchTargets.push_back(target);
        }
    }

    summary.hasLoopCandidate = summary.branchBackedgeCandidates != 0;
    return summary;
}

} // namespace hyperlang::hil
