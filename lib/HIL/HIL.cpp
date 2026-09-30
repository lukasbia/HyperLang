#include "hyper/HIL/HIL.h"
#include <utility>
namespace hyper::hil {
Module::Module(std::string name):name_(std::move(name)){}
const std::string& Module::name()const noexcept{return name_;}
void Module::addFunction(Function f){functions_.push_back(std::move(f));}
std::vector<Function>& Module::functions()noexcept{return functions_;}
const std::vector<Function>& Module::functions()const noexcept{return functions_;}
Function::Function(std::string name):name_(std::move(name)){}
const std::string& Function::name()const noexcept{return name_;}
void Function::addBlock(BasicBlock b){blocks_.push_back(std::move(b));}
std::vector<BasicBlock>& Function::blocks()noexcept{return blocks_;}
const std::vector<BasicBlock>& Function::blocks()const noexcept{return blocks_;}
BasicBlock::BasicBlock(std::string name):name_(std::move(name)){}
const std::string& BasicBlock::name()const noexcept{return name_;}
void BasicBlock::append(Instruction i){instructions_.push_back(std::move(i));}
std::vector<Instruction>& BasicBlock::instructions()noexcept{return instructions_;}
const std::vector<Instruction>& BasicBlock::instructions()const noexcept{return instructions_;}
Instruction::Instruction(Opcode opcode):opcode_(opcode){}
Opcode Instruction::opcode()const noexcept{return opcode_;}
void Instruction::addOperand(Value v){operands_.push_back(std::move(v));}
const std::vector<Value>& Instruction::operands()const noexcept{return operands_;}
const char* opcodeName(Opcode o)noexcept{switch(o){case Opcode::Constant:return "constant";case Opcode::Move:return "move";case Opcode::Copy:return "copy";case Opcode::Load:return "load";case Opcode::Store:return "store";case Opcode::Add:return "add";case Opcode::Subtract:return "sub";case Opcode::Multiply:return "mul";case Opcode::Divide:return "div";case Opcode::Remainder:return "rem";case Opcode::Branch:return "br";case Opcode::ConditionalBranch:return "condbr";case Opcode::Call:return "call";case Opcode::Return:return "return";case Opcode::Allocate:return "alloc";case Opcode::Deallocate:return "dealloc";case Opcode::Retain:return "retain";case Opcode::Release:return "release";case Opcode::Phi:return "phi";case Opcode::Cast:return "cast";case Opcode::Trap:return "trap";default:return "other";}}
bool isTerminator(Opcode o)noexcept{return o==Opcode::Branch||o==Opcode::ConditionalBranch||o==Opcode::Return||o==Opcode::Trap;}
bool isMemoryOperation(Opcode o)noexcept{return o==Opcode::Load||o==Opcode::Store||o==Opcode::Allocate||o==Opcode::Deallocate;}
bool isPureOperation(Opcode o)noexcept{return o==Opcode::Constant||o==Opcode::Move||o==Opcode::Copy||o==Opcode::Add||o==Opcode::Subtract||o==Opcode::Multiply||o==Opcode::Divide||o==Opcode::Remainder||o==Opcode::CompareEqual||o==Opcode::CompareNotEqual||o==Opcode::CompareLess||o==Opcode::CompareLessEqual||o==Opcode::CompareGreater||o==Opcode::CompareGreaterEqual||o==Opcode::Cast;}
bool isOwnershipOperation(Opcode o)noexcept{return o==Opcode::Retain||o==Opcode::Release;}
std::size_t instructionCount(const Module&m)noexcept{std::size_t n=0;for(const auto&f:m.functions())for(const auto&b:f.blocks())n+=b.instructions().size();return n;}
std::size_t basicBlockCount(const Module&m)noexcept{std::size_t n=0;for(const auto&f:m.functions())n+=f.blocks().size();return n;}
std::size_t functionCount(const Module&m)noexcept{return m.functions().size();}
std::size_t operandCount(const Instruction&i)noexcept{return i.operands().size();}
} // namespace hyper::hil
