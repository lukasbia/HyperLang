#include "hyperlang/HIL/Optimization/HILOptimizer.h"

#include <array>
#include <functional>
#include <utility>

namespace hyperlang::hil {

namespace {

struct Pass {
    const char* name;
    bool OptimizationOptions::*enabled;
    bool (*run)(Module&);
};

}

OptimizationStats optimizeModule(Module& module, const OptimizationOptions& options) {
    static constexpr std::array<Pass, 14> pipeline{{
        {"ConstantFolding", &OptimizationOptions::constantFolding, &runConstantFolding},
        {"ConstantPropagation", &OptimizationOptions::constantPropagation, &runConstantPropagation},
        {"CopyPropagation", &OptimizationOptions::copyPropagation, &runCopyPropagation},
        {"AlgebraicSimplification", &OptimizationOptions::algebraicSimplification, &runAlgebraicSimplification},
        {"CommonSubexpressionElimination", &OptimizationOptions::commonSubexpressionElimination, &runCommonSubexpressionElimination},
        {"DeadCodeElimination", &OptimizationOptions::deadCodeElimination, &runDeadCodeElimination},
        {"BranchSimplification", &OptimizationOptions::branchSimplification, &runBranchSimplification},
        {"Peephole", &OptimizationOptions::peephole, &runPeephole},
        {"LoadStoreSimplification", &OptimizationOptions::loadStoreSimplification, &runLoadStoreSimplification},
        {"StrengthReduction", &OptimizationOptions::strengthReduction, &runStrengthReduction},
        {"CompareSimplification", &OptimizationOptions::compareSimplification, &runCompareSimplification},
        {"InstructionCombining", &OptimizationOptions::instructionCombining, &runInstructionCombining},
        {"RedundantMoveElimination", &OptimizationOptions::redundantMoveElimination, &runRedundantMoveElimination},
        {"FunctionCleanup", &OptimizationOptions::functionCleanup, &runFunctionCleanup},
    }};

    OptimizationStats stats;
    constexpr std::size_t maxIterations = 8;

    for (std::size_t iteration = 0; iteration < maxIterations; ++iteration) {
        bool changedThisRound = false;
        ++stats.iterations;

        for (const auto& pass : pipeline) {
            if (!(options.*(pass.enabled))) continue;
            if (!pass.run(module)) continue;
            changedThisRound = true;
            ++stats.transformations;
            stats.changedPasses.emplace_back(pass.name);
        }

        if (!changedThisRound) break;
    }

    return stats;
}

void runOptimization(Module& module) {
    (void)optimizeModule(module);
}

} // namespace hyperlang::hil
