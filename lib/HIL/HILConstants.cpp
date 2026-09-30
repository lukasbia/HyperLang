#include "hyper/HIL/HIL.h"
#include <string>
namespace hyper::hil {
bool isConstantProducer(const Instruction&i){return i.opcode()==Opcode::Constant;}
Value makeConstant(std::uint32_t id,std::string type,std::string name){return Value{id,std::move(type),std::move(name)};}
}