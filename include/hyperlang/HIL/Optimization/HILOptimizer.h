#pragma once

#include "hyperlang/HIL/HIL.h"
#include <cstddef>
#include <string>
#include <vector>

namespace hyperlang::hil {

struct OptimizationOptions {
    bool constantFolding = true;
    bool constantPropagation = true;
    bool sparseConditionalConstantPropagation = true;
    bool copyPropagation = true;
    bool algebraicSimplification = true;
    bool commonSubexpressionElimination = true;
    bool localValueNumbering = true;
    bool deadCodeElimination = true;
    bool deadStoreElimination = true;
    bool storeForwarding = true;
    bool redundantLoadElimination = true;
    bool branchSimplification = true;
    bool simplifyCFG = true;
    bool peephole = true;
    bool loadStoreSimplification = true;
    bool strengthReduction = true;
    bool compareSimplification = true;
    bool compareConstantFolding = true;
    bool instructionCombining = true;
    bool redundantMoveElimination = true;
    bool literalCanonicalization = true;
    bool functionCleanup = true;
};

struct OptimizationStats {
    std::size_t iterations = 0;
    std::size_t transformations = 0;
    std::vector<std::string> changedPasses;
};

struct CFGSummary { std::size_t branchCount = 0; std::size_t conditionalBranchCount = 0; std::size_t terminatorCount = 0; bool structurallyValid = true; };
struct LoopSummary { std::size_t branchBackedgeCandidates = 0; bool hasLoopCandidate = false; };
struct AliasSummary { std::size_t loadCount = 0; std::size_t storeCount = 0; std::size_t exactVariableMatches = 0; };
struct RangeSummary { bool hasIntegerLiterals = false; long long minimum = 0; long long maximum = 0; };
struct CostSummary { std::size_t instructionCount = 0; std::size_t estimatedCost = 0; };

bool runConstantFolding(Module& module);
bool runConstantPropagation(Module& module);
bool runSparseConditionalConstantPropagation(Module& module);
bool runCopyPropagation(Module& module);
bool runAlgebraicSimplification(Module& module);
bool runCommonSubexpressionElimination(Module& module);
bool runLocalValueNumbering(Module& module);
bool runDeadCodeElimination(Module& module);
bool runDeadStoreElimination(Module& module);
bool runStoreForwarding(Module& module);
bool runRedundantLoadElimination(Module& module);
bool runBranchSimplification(Module& module);
bool runSimplifyCFG(Module& module);
bool runPeephole(Module& module);
bool runLoadStoreSimplification(Module& module);
bool runStrengthReduction(Module& module);
bool runCompareSimplification(Module& module);
bool runCompareConstantFolding(Module& module);
bool runInstructionCombining(Module& module);
bool runRedundantMoveElimination(Module& module);
bool runLiteralCanonicalization(Module& module);
bool runFunctionCleanup(Module& module);

CFGSummary analyzeCFG(const Module& module);
LoopSummary analyzeLoops(const Module& module);
AliasSummary analyzeAliases(const Module& module);
RangeSummary analyzeRanges(const Module& module);
CostSummary estimateOptimizationCost(const Module& module);
bool runOptimizationAnalyses(const Module& module);
bool verifyOptimizedModule(const Module& module, std::string* error = nullptr);

OptimizationStats optimizeModule(Module& module, const OptimizationOptions& options = {});
void runOptimization(Module& module);

} // namespace hyperlang::hil
