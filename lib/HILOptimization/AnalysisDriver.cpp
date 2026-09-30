#include "hyperlang/HIL/Optimization/HILOptimizer.h"

namespace hyperlang::hil {

bool runOptimizationAnalyses(const Module& module) {
    const auto cfg = analyzeCFG(module);
    const auto loops = analyzeLoops(module);
    const auto aliases = analyzeAliases(module);
    const auto range = analyzeRanges(module);
    const auto cost = estimateOptimizationCost(module);

    return cfg.structurallyValid &&
           (!loops.hasLoopCandidate || loops.branchBackedgeCandidates <= cost.instructionCount) &&
           aliases.loadCount + aliases.storeCount <= cost.instructionCount &&
           (!range.hasIntegerLiterals || range.minimum <= range.maximum);
}

} // namespace hyperlang::hil
