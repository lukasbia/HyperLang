#include "hyper/HIL/HIL.h"
#include <sstream>

namespace hyper::hil {

std::string serializeModule(const Module& module) {
    std::ostringstream out;
    out << "module " << module.name() << '\n';
    for (const auto& function : module.functions()) {
        out << "  func " << function.name() << '\n';
        for (const auto& block : function.blocks()) {
            out << "    block " << block.name() << '\n';
            for (const auto& instruction : block.instructions()) {
                out << "      " << opcodeName(instruction.opcode());
                for (const auto& operand : instruction.operands()) out << ' ' << operand.id;
                if (instruction.hasResult()) out << " -> %" << instruction.result().id;
                out << '\n';
            }
        }
    }
    return out.str();
}

} // namespace hyper::hil
