#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool isCast(const Instruction&i){return i.opcode()==Opcode::Cast;}
bool castHasOperands(const Instruction&i){return isCast(i)&&i.operands().size()>=1;}
}