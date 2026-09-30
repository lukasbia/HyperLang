#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <limits>

namespace hyperlang::hil {

namespace {

bool evaluate(Opcode op, long long a, long long b, long long& out) {
    switch (op) {
        case Opcode::Add:
            if ((b > 0 && a > std::numeric_limits<long long>::max() - b) ||
                (b < 0 && a < std::numeric_limits<long long>::min() - b)) return false;
            out = a + b; return true;
        case Opcode::Subtract:
            if ((b < 0 && a > std::numeric_limits<long long>::max() + b) ||
                (b > 0 && a < std::numeric_limits<long long>::min() + b)) return false;
            out = a - b; return true;
        case Opcode::Multiply:
            if (a != 0 && b != 0) {
                if (a == -1 && b == std::numeric_limits<long long>::min()) return false;
                if (b == -1 && a == std::numeric_limits<long long>::min()) return false;
                if (a > 0 && b > 0 && a > std::numeric_limits<long long>::max() / b) return false;
                if (a < 0 && b < 0 && a < std::numeric_limits<long long>::max() / b) return false;
                if (a > 0 && b < 0 && b < std::numeric_limits<long long>::min() / a) return false;
                if (a < 0 && b > 0 && a < std::numeric_limits<long long>::min() / b) return false;
            }
            out = a * b; return true;
        case Opcode::Divide:
            if (b == 0 || (a == std::numeric_limits<long long>::min() && b == -1)) return false;
            out = a / b; return true;
        default:
            return false;
    }
}

}

bool runConstantFolding(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        for (auto& instruction : function.instructions) {
            auto ops = optimization::operandsOf(instruction);
            if (!optimization::isBinaryArithmetic(instruction.opcode) || ops.size() != 2) continue;

            long long lhs = 0, rhs = 0, result = 0;
            if (!optimization::parseInteger(ops[0], lhs) || !optimization::parseInteger(ops[1], rhs)) continue;
            if (!evaluate(instruction.opcode, lhs, rhs, result)) continue;

            instruction.opcode = Opcode::LoadLiteral;
            optimization::setOperands(instruction, {std::to_string(result)});
            changed = true;
        }
    }
    return changed;
}

} // namespace hyperlang::hil
