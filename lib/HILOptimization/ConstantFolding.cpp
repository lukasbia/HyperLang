#include "hyper/HIL/HIL.h"

namespace hyper::hil::optimization {

static bool isBinaryConstantCandidate(Opcode opcode) noexcept {
    return opcode == Opcode::Add || opcode == Opcode::Subtract || opcode == Opcode::Multiply || opcode == Opcode::Divide || opcode == Opcode::Remainder;
}

bool runConstantFolding(Module& module) noexcept {
    bool changed = false;
    for (auto& function : module.functions()) {
        for (auto& block : function.blocks()) {
            auto& instructions = block.instructions();
            for (auto& instruction : instructions) {
                if (!isBinaryConstantCandidate(instruction.opcode()) || instruction.operands().size() != 2) continue;
                const auto& lhs = instruction.operands()[0];
                const auto& rhs = instruction.operands()[1];
                if (lhs.name.empty() || rhs.name.empty()) continue;
                try {
                    const long long a = std::stoll(lhs.name);
                    const long long b = std::stoll(rhs.name);
                    long long result = 0;
                    switch (instruction.opcode()) {
                        case Opcode::Add: result = a + b; break;
                        case Opcode::Subtract: result = a - b; break;
                        case Opcode::Multiply: result = a * b; break;
                        case Opcode::Divide: if (b == 0) continue; result = a / b; break;
                        case Opcode::Remainder: if (b == 0) continue; result = a % b; break;
                        default: continue;
                    }
                    instruction.setOpcode(Opcode::Constant);
                    instruction.setOperands({Value{0, lhs.type, std::to_string(result)}});
                    changed = true;
                } catch (...) {
                }
            }
        }
    }
    return changed;
}

} // namespace hyper::hil::optimization
