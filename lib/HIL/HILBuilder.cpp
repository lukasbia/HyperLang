#include "hyper/HIL/HIL.h"
namespace hyper::hil {
BasicBlock buildBlock(const std::string&name,std::vector<Instruction> instructions){BasicBlock b(name);for(auto&i:instructions)b.append(std::move(i));return b;}
Instruction makeInstruction(Opcode op,std::vector<Value> operands){Instruction i(op);for(auto&v:operands)i.addOperand(std::move(v));return i;}
}