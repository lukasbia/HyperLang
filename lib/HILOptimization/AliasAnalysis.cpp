#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil {

AliasSummary analyzeAliases(const Module& module) {
    AliasSummary summary;
    for (const auto& function : module.functions) {
        std::unordered_map<std::string, std::size_t> loads;
        for (const auto& instruction : function.instructions) {
            const auto ops = optimization::operandsOf(instruction);
            if (instruction.opcode == Opcode::LoadVariable && !ops.empty()) {
                ++summary.loadCount;
                ++loads[ops.front()];
            } else if (instruction.opcode == Opcode::StoreVariable && !ops.empty()) {
                ++summary.storeCount;
                summary.exactVariableMatches += loads[ops.front()];
            }
        }
    }
    return summary;
}

} // namespace hyperlang::hil
