#include "hyper/LLCVM/IRGenerator.h"

namespace hyper::llcvm {

CompiledLine IRGenerator::generateLine(std::string_view source, std::size_t line,
                                       std::string_view file) const {
    CompiledLine result;
    result.sourceLine = line;
    result.source = std::string(source);
    Instruction instruction;
    instruction.opcode = Opcode::Invalid;
    instruction.location.file = std::string(file);
    instruction.location.line = line;
    instruction.location.column = 1;
    result.instructions.push_back(std::move(instruction));
    return result;
}

} // namespace hyper::llcvm
