#include "hyper/HIL/HIL.h"
#include "hyper/HILOptimization/HILOptimization.h"
#include <charconv>
#include <string>

namespace hyper::hil::optimization {

static bool parseInteger(const Value& value, long long& result) {
    if (value.name.empty()) return false;
    const char* first = value.name.data();
    const char* last = first + value.name.size();
    auto parsed = std::from_chars(first, last, result);
    return parsed.ec == std::errc{} && parsed.ptr == last;
}

static bool foldBinary(Instruction& instruction) {
    if (instruction.operands().size() != 2) return false;
    long long lhs = 0, rhs = 0;
    if (!parseInteger(instruction.operands()[0], lhs) || !parseInteger(instruction.operands()[1], rhs)) return false;

    long long value = 0;
    switch (instruction.opcode()) {
        case Opcode::Add: value = lhs + rhs; break;
        case Opcode::Subtract: value = lhs - rhs; break;
        case Opcode::Multiply: value = lhs * rhs; break;
        case Opcode::Divide: if (rhs == 0) return false; value = lhs / rhs; break;
        case Opcode::Remainder: if (rhs == 0) return false; value = lhs % rhs; break;
        default: return false;
    }

    Value constant;
    if (instruction.hasResult()) constant = instruction.result();
    constant.name = std::to_string(value);
    instruction.setOpcode(Opcode::Constant);
    instruction.setOperands({constant});
    return true;
}

bool runConstantFolding(Module& module) {
    bool changed = false;
    for (auto& function : module.functions()) {
        for (auto& block : function.blocks()) {
            for (auto& instruction : block.instructions()) changed |= foldBinary(instruction);
        }
    }
    return changed;
}

} // namespace hyper::hil::optimization
