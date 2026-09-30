#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool touchesMemory(const Instruction&i){return isMemoryOperation(i.opcode());}
std::size_t countMemoryOperations(const BasicBlock&b){std::size_t n=0;for(const auto&i:b.instructions())if(touchesMemory(i))++n;return n;}
}