#include "hyper/HIL/HIL.h"
namespace hyper::hil {
std::size_t countTerminators(const BasicBlock&b){std::size_t n=0;for(const auto&i:b.instructions())if(isTerminator(i.opcode()))++n;return n;}
bool isSingleExitBlock(const BasicBlock&b){return countTerminators(b)==1;}
}