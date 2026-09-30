#include "hyper/LLCVM/LLCVM.h"

namespace hyper::llcvm {

bool verifyLine(const CompiledLine& line, std::string* error) {
    if (line.sourceLine == 0) {
        if (error) *error = "source line must be one-based";
        return false;
    }
    for (const auto& instruction : line.instructions) {
        if (instruction.opcode == Opcode::Invalid) {
            if (error) *error = "invalid LLCVM opcode";
            return false;
        }
        if (instruction.location.line != 0 && instruction.location.line != line.sourceLine) {
            if (error) *error = "instruction source line does not match compiled line";
            return false;
        }
    }
    return true;
}

} // namespace hyper::llcvm
