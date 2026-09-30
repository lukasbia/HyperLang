#include "hyperlang/HIL/Optimization/OptimizationUtils.h"

#include <charconv>
#include <cctype>

namespace hyperlang::hil::optimization {

std::string trim(std::string_view text) {
    std::size_t first = 0;
    while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first]))) ++first;
    std::size_t last = text.size();
    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1]))) --last;
    return std::string(text.substr(first, last - first));
}

std::vector<std::string> splitOperands(std::string_view text) {
    std::vector<std::string> result;
    std::string current;
    for (char ch : text) {
        if (ch == ',') {
            if (!trim(current).empty()) result.push_back(trim(current));
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (!trim(current).empty()) result.push_back(trim(current));
    return result;
}

std::string joinOperands(const std::vector<std::string>& operands) {
    std::string result;
    for (std::size_t i = 0; i < operands.size(); ++i) {
        if (i) result.push_back(',');
        result += operands[i];
    }
    return result;
}

std::vector<std::string> operandsOf(const Instruction& instruction) {
    if (!instruction.operands.empty()) return instruction.operands;
    return splitOperands(instruction.operand);
}

void setOperands(Instruction& instruction, std::vector<std::string> operands) {
    instruction.operands = std::move(operands);
    instruction.operand = joinOperands(instruction.operands);
}

bool parseInteger(std::string_view text, long long& value) {
    const std::string cleaned = trim(text);
    if (cleaned.empty()) return false;
    const char* begin = cleaned.data();
    const char* end = begin + cleaned.size();
    const auto parsed = std::from_chars(begin, end, value, 10);
    return parsed.ec == std::errc{} && parsed.ptr == end;
}

bool isBinaryArithmetic(Opcode opcode) noexcept {
    return opcode == Opcode::Add || opcode == Opcode::Subtract ||
           opcode == Opcode::Multiply || opcode == Opcode::Divide;
}

bool isTerminator(Opcode opcode) noexcept {
    return opcode == Opcode::FunctionEnd || opcode == Opcode::Return ||
           opcode == Opcode::Branch || opcode == Opcode::BranchIf;
}

bool usesSideEffects(Opcode opcode) noexcept {
    return opcode == Opcode::StoreVariable || opcode == Opcode::Call ||
           opcode == Opcode::Return || opcode == Opcode::Branch ||
           opcode == Opcode::BranchIf || opcode == Opcode::FunctionBegin ||
           opcode == Opcode::FunctionEnd;
}

bool isPure(Opcode opcode) noexcept {
    return opcode != Opcode::Invalid && !usesSideEffects(opcode);
}

bool definesValue(const Instruction& instruction) noexcept {
    return !instruction.result.empty();
}

} // namespace hyperlang::hil::optimization
