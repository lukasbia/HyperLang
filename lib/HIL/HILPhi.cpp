#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool isPhi(const Instruction&i){return i.opcode()==Opcode::Phi;}
std::size_t phiIncomingCount(const Instruction&i){return isPhi(i)?i.operands().size():0;}
}