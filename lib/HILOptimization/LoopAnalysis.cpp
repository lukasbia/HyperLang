#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

LoopSummary analyzeLoops(const Module& module) {
    LoopSummary summary;
    for (const auto& function : module.functions) {
        std::vector<std::string> seenBlocks;
        for (const auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::Branch && instruction.opcode != Opcode::BranchIf) continue;
            const auto ops = optimization::operandsOf(instruction);
            if (ops.empty()) continue;
            if (std::find(seenBlocks.begin(), seenBlocks.end(), ops.back()) != seenBlocks.end()) {
                ++summary.branchBackedgeCandidates;
                summary.hasLoopCandidate = true;
            }
            seenBlocks.push_back(ops.back());
        }
    }
    return summary;
}

} // namespace hyperlang::hil
