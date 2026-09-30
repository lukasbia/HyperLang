#include "hyperlang/LSC/Optimization/LSCOptimizer.h"
#include <algorithm>
namespace hyperlang::lsc {
void optimize(IR& ir) {
    std::vector<IRInstruction> optimized;
    optimized.reserve(ir.instructions.size());
    for (const auto& instruction : ir.instructions) {
        if (!optimized.empty() && instruction.opcode == "nop") continue;
        optimized.push_back(instruction);
    }
    ir.instructions.swap(optimized);
    ++ir.revision;
}
}
