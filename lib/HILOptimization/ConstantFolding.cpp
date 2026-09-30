#include "hyperlang/HIL/HIL.h"

#include <cstdlib>
#include <limits>
#include <string>

namespace hyperlang::hil {
namespace {

bool parseInteger(const std::string& text, long long& value) {
    if (text.empty()) return false;
    char* end = nullptr;
    const long long parsed = std::strtoll(text.c_str(), &end, 10);
    if (end == text.c_str() || *end != '\0') return false;
    value = parsed;
    return true;
}

bool foldBinary(Instruction& instruction, const Instruction& lhs, const Instruction& rhs) {
    long long left = 0;
    long long right = 0;
    if (!parseInteger(lhs.operand, left) || !parseInteger(rhs.operand, right)) return false;

    long long result = 0;
    switch (instruction.opcode) {
        case Opcode::Add:
            if ((right > 0 && left > std::numeric_limits<long long>::max() - right) ||
                (right < 0 && left < std::numeric_limits<long long>::min() - right)) return false;
            result = left + right;
            break;
        case Opcode::Subtract:
            if ((right < 0 && left > std::numeric_limits<long long>::max() + right) ||
                (right > 0 && left < std::numeric_limits<long long>::min() + right)) return false;
            result = left - right;
            break;
        case Opcode::Multiply:
            if (left != 0 && right != 0) {
                if (left == -1 && right == std::numeric_limits<long long>::min()) return false;
                if (right == -1 && left == std::numeric_limits<long long>::min()) return false;
                if (left > 0 && right > 0 && left > std::numeric_limits<long long>::max() / right) return false;
                if (left < 0 && right < 0 && left < std::numeric_limits<long long>::max() / right) return false;
                if (left > 0 && right < 0 && right < std::numeric_limits<long long>::min() / left) return false;
                if (left < 0 && right > 0 && left < std::numeric_limits<long long>::min() / right) return false;
            }
            result = left * right;
            break;
        case Opcode::Divide:
            if (right == 0 || (left == std::numeric_limits<long long>::min() && right == -1)) return false;
            result = left / right;
            break;
        default:
            return false;
    }

    instruction.opcode = Opcode::LoadLiteral;
    instruction.operand = std::to_string(result);
    return true;
}

} // namespace

void optimize(Module& module) {
    for (Function& function : module.functions) {
        std::vector<Instruction> folded;
        folded.reserve(function.instructions.size());

        for (std::size_t index = 0; index < function.instructions.size(); ++index) {
            Instruction current = function.instructions[index];
            if (current.opcode == Opcode::Add || current.opcode == Opcode::Subtract ||
                current.opcode == Opcode::Multiply || current.opcode == Opcode::Divide) {
                if (index >= 2 && folded.size() >= 2) {
                    const Instruction& lhs = folded[folded.size() - 2];
                    const Instruction& rhs = folded[folded.size() - 1];
                    if (lhs.opcode == Opcode::LoadLiteral && rhs.opcode == Opcode::LoadLiteral &&
                        foldBinary(current, lhs, rhs)) {
                        folded.pop_back();
                        folded.pop_back();
                    }
                }
            }
            folded.push_back(std::move(current));
        }

        function.instructions = std::move(folded);
    }
}

} // namespace hyperlang::hil
