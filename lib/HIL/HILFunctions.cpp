#include "hyper/HIL/HIL.h"
namespace hyper::hil {
std::size_t functionInstructionCount(const Function&f){std::size_t n=0;for(const auto&b:f.blocks())n+=b.instructions().size();return n;}
bool functionIsEmpty(const Function&f){return f.blocks().empty();}
}