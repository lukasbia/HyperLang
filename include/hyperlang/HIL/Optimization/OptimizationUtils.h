#pragma once

#include "hyperlang/HIL/HIL.h"
#include <string>
#include <string_view>
#include <vector>

namespace hyperlang::hil::optimization {

std::string trim(std::string_view text);
std::vector<std::string> splitOperands(std::string_view text);
std::string joinOperands(const std::vector<std::string>& operands);
std::vector<std::string> operandsOf(const Instruction& instruction);
void setOperands(Instruction& instruction, std::vector<std::string> operands);

bool parseInteger(std::string_view text, long long& value);
bool isBinaryArithmetic(Opcode opcode) noexcept;
bool isPure(Opcode opcode) noexcept;
bool isTerminator(Opcode opcode) noexcept;
bool definesValue(const Instruction& instruction) noexcept;
bool usesSideEffects(Opcode opcode) noexcept;

} // namespace hyperlang::hil::optimization
