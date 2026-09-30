#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <limits>

namespace hyperlang::hil {

RangeSummary analyzeRanges(const Module& module) {
    RangeSummary summary;
    summary.minimum = std::numeric_limits<long long>::max();
    summary.maximum = std::numeric_limits<long long>::min();

    for (const auto& function : module.functions) {
        for (const auto& instruction : function.instructions) {
            if (instruction.opcode != Opcode::LoadLiteral) continue;
            for (const auto& operand : optimization::operandsOf(instruction)) {
                long long value = 0;
                if (!optimization::parseInteger(operand, value)) continue;
                summary.hasIntegerLiterals = true;
                if (value < summary.minimum) summary.minimum = value;
                if (value > summary.maximum) summary.maximum = value;
            }
        }
    }

    if (!summary.hasIntegerLiterals) summary.minimum = summary.maximum = 0;
    return summary;
}

} // namespace hyperlang::hil
