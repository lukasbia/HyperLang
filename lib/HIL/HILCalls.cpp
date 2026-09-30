#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool isCall(const Instruction&i){return i.opcode()==Opcode::Call;}
std::size_t callArgumentCount(const Instruction&i){return isCall(i)?i.operands().size():0;}
}