#include "hyper/LLCVM/IRGenerator.h"
#include <cctype>
#include <sstream>
#include <string>

namespace hyper::llcvm {

static Opcode classify(std::string_view word) {
    if (word == "let" || word == "const") return Opcode::Constant;
    if (word == "move") return Opcode::Move;
    if (word == "load") return Opcode::Load;
    if (word == "store") return Opcode::Store;
    if (word == "add" || word == "+") return Opcode::Add;
    if (word == "sub" || word == "-") return Opcode::Sub;
    if (word == "mul" || word == "*") return Opcode::Mul;
    if (word == "div" || word == "/") return Opcode::Div;
    if (word == "compare" || word == "==" || word == "!=") return Opcode::Compare;
    if (word == "branch" || word == "if") return Opcode::Branch;
    if (word == "call") return Opcode::Call;
    if (word == "return") return Opcode::Return;
    if (word == "retain") return Opcode::Retain;
    if (word == "release") return Opcode::Release;
    return Opcode::Invalid;
}

CompiledLine IRGenerator::generateLine(std::string_view source, std::size_t line,
                                       std::string_view file) const {
    CompiledLine result;
    result.sourceLine = line;
    result.source = std::string(source);

    std::istringstream stream(std::string(source));
    std::string word;
    std::size_t column = 1;
    while (stream >> word) {
        Instruction instruction;
        instruction.opcode = classify(word);
        instruction.location.file = std::string(file);
        instruction.location.line = line;
        instruction.location.column = column;
        instruction.operands.push_back(word);
        result.instructions.push_back(std::move(instruction));
        column += word.size() + 1;
    }

    if (result.instructions.empty()) {
        Instruction instruction;
        instruction.opcode = Opcode::Invalid;
        instruction.location.file = std::string(file);
        instruction.location.line = line;
        instruction.location.column = 1;
        result.instructions.push_back(std::move(instruction));
    }
    return result;
}

} // namespace hyper::llcvm
