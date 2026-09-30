#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool isValidInstruction(const Instruction&i){if(i.opcode()==Opcode::Nop)return true;if(i.opcode()==Opcode::Call)return !i.operands().empty();if(i.opcode()==Opcode::ConditionalBranch)return i.operands().size()>=3;return true;}
std::size_t countInvalidInstructions(const BasicBlock&b){std::size_t n=0;for(const auto&i:b.instructions())if(!isValidInstruction(i))++n;return n;}
}