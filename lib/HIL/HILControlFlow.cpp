#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool hasTerminator(const BasicBlock& b){return !b.instructions().empty()&&isTerminator(b.instructions().back().opcode());}
void ensureTerminator(BasicBlock& b){if(!hasTerminator(b))b.append(Instruction(Opcode::Trap));}
}