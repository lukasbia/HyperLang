#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include <array>

namespace hyperlang::hil {

namespace optimization {
bool runConstantPropagation(Module&);
bool runCopyPropagation(Module&);
bool runAlgebraicSimplification(Module&);
bool runCommonSubexpressionElimination(Module&);
bool runDeadCodeElimination(Module&);
bool runBranchSimplification(Module&);
bool runPeephole(Module&);
bool runLoadStoreSimplification(Module&);
bool runStrengthReduction(Module&);
bool runCompareSimplification(Module&);
bool runInstructionCombining(Module&);
bool runRedundantMoveElimination(Module&);
}

bool runConstantPropagation(Module& m) { return optimization::runConstantPropagation(m); }
bool runCopyPropagation(Module& m) { return optimization::runCopyPropagation(m); }
bool runAlgebraicSimplification(Module& m) { return optimization::runAlgebraicSimplification(m); }
bool runCommonSubexpressionElimination(Module& m) { return optimization::runCommonSubexpressionElimination(m); }
bool runDeadCodeElimination(Module& m) { return optimization::runDeadCodeElimination(m); }
bool runBranchSimplification(Module& m) { return optimization::runBranchSimplification(m); }
bool runPeephole(Module& m) { return optimization::runPeephole(m); }
bool runLoadStoreSimplification(Module& m) { return optimization::runLoadStoreSimplification(m); }
bool runStrengthReduction(Module& m) { return optimization::runStrengthReduction(m); }
bool runCompareSimplification(Module& m) { return optimization::runCompareSimplification(m); }
bool runInstructionCombining(Module& m) { return optimization::runInstructionCombining(m); }
bool runRedundantMoveElimination(Module& m) { return optimization::runRedundantMoveElimination(m); }

namespace {

struct Pass {
    const char* name;
    bool OptimizationOptions::*enabled;
    bool (*run)(Module&);
};

}

OptimizationStats optimizeModule(Module& module, const OptimizationOptions& options) {
    static constexpr std::array<Pass, 22> pipeline{{
        {"ConstantFolding", &OptimizationOptions::constantFolding, &runConstantFolding},
        {"ConstantPropagation", &OptimizationOptions::constantPropagation, &runConstantPropagation},
        {"SparseConditionalConstantPropagation", &OptimizationOptions::sparseConditionalConstantPropagation, &runSparseConditionalConstantPropagation},
        {"CopyPropagation", &OptimizationOptions::copyPropagation, &runCopyPropagation},
        {"AlgebraicSimplification", &OptimizationOptions::algebraicSimplification, &runAlgebraicSimplification},
        {"CommonSubexpressionElimination", &OptimizationOptions::commonSubexpressionElimination, &runCommonSubexpressionElimination},
        {"LocalValueNumbering", &OptimizationOptions::localValueNumbering, &runLocalValueNumbering},
        {"DeadCodeElimination", &OptimizationOptions::deadCodeElimination, &runDeadCodeElimination},
        {"DeadStoreElimination", &OptimizationOptions::deadStoreElimination, &runDeadStoreElimination},
        {"StoreForwarding", &OptimizationOptions::storeForwarding, &runStoreForwarding},
        {"RedundantLoadElimination", &OptimizationOptions::redundantLoadElimination, &runRedundantLoadElimination},
        {"BranchSimplification", &OptimizationOptions::branchSimplification, &runBranchSimplification},
        {"SimplifyCFG", &OptimizationOptions::simplifyCFG, &runSimplifyCFG},
        {"Peephole", &OptimizationOptions::peephole, &runPeephole},
        {"LoadStoreSimplification", &OptimizationOptions::loadStoreSimplification, &runLoadStoreSimplification},
        {"StrengthReduction", &OptimizationOptions::strengthReduction, &runStrengthReduction},
        {"CompareSimplification", &OptimizationOptions::compareSimplification, &runCompareSimplification},
        {"CompareConstantFolding", &OptimizationOptions::compareConstantFolding, &runCompareConstantFolding},
        {"InstructionCombining", &OptimizationOptions::instructionCombining, &runInstructionCombining},
        {"RedundantMoveElimination", &OptimizationOptions::redundantMoveElimination, &runRedundantMoveElimination},
        {"LiteralCanonicalization", &OptimizationOptions::literalCanonicalization, &runLiteralCanonicalization},
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
