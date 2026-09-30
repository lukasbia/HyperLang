#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

namespace hyperlang::hil {

bool verifyOptimizedModule(const Module& module, std::string* error) {
    for (const auto& function : module.functions) {
        if (function.name.empty()) {
            if (error) *error = "HIL function has an empty name";
            return false;
        }
        if (function.instructions.empty()) {
            if (error) *error = "HIL function has no instructions";
            return false;
        }

        bool hasBegin = function.instructions.front().opcode == Opcode::FunctionBegin;
        bool hasEnd = function.instructions.back().opcode == Opcode::FunctionEnd;
        if (!hasBegin || !hasEnd) {
            if (error) *error = "HIL function boundaries are malformed";
            return false;
        }

        for (const auto& instruction : function.instructions) {
            if (instruction.opcode == Opcode::FunctionBegin && instruction.operand != function.name) {
                if (error) *error = "function-begin name does not match function";
                return false;
            }
            if (instruction.opcode == Opcode::FunctionEnd && instruction.operand != function.name) {
                if (error) *error = "function-end name does not match function";
                return false;
            }
        }
    }
    return true;
}

} // namespace hyperlang::hil
