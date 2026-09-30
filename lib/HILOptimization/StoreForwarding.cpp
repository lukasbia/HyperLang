#include "hyperlang/HIL/Optimization/OptimizationUtils.h"
#include <unordered_map>

namespace hyperlang::hil {

bool runStoreForwarding(Module& module) {
    bool changed = false;
    for (auto& function : module.functions) {
        std::unordered_map<std::string, std::string> stored;
        for (auto& instruction : function.instructions) {
            const auto ops = optimization::operandsOf(instruction);
            if (instruction.opcode == Opcode::StoreVariable && ops.size() == 2) {
                long long ignored = 0;
                if (optimization::parseInteger(ops[1], ignored)) stored[ops[0]] = ops[1];
                else stored.erase(ops[0]);
            } else if (instruction.opcode == Opcode::LoadVariable && ops.size() == 1) {
                const auto it = stored.find(ops[0]);
                if (it != stored.end()) {
                    instruction.opcode = Opcode::LoadLiteral;
                    optimization::setOperands(instruction, {it->second});
                    changed = true;
                }
            } else if (instruction.opcode == Opcode::Call) {
                stored.clear();
            }
        }
    }
    return changed;
}

} // namespace hyperlang::hil
