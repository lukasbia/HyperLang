#include "hyper/HIL/HIL.h"
namespace hyper::hil {
int ownershipDelta(const Instruction&i){if(i.opcode()==Opcode::Retain)return 1;if(i.opcode()==Opcode::Release)return -1;return 0;}
int blockOwnershipBalance(const BasicBlock&b){int n=0;for(const auto&i:b.instructions())n+=ownershipDelta(i);return n;}
}