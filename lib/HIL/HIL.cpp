#include "hyper/HIL/HIL.h"
#include <utility>

namespace hyper::hil {

Module::Module(std::string name) : name_(std::move(name)) {}
const std::string& Module::name() const noexcept { return name_; }
void Module::addFunction(Function function) { functions_.push_back(std::move(function)); }
std::vector<Function>& Module::functions() noexcept { return functions_; }
const std::vector<Function>& Module::functions() const noexcept { return functions_; }

Function::Function(std::string name) : name_(std::move(name)) {}
const std::string& Function::name() const noexcept { return name_; }
void Function::addBlock(BasicBlock block) { blocks_.push_back(std::move(block)); }
std::vector<BasicBlock>& Function::blocks() noexcept { return blocks_; }
const std::vector<BasicBlock>& Function::blocks() const noexcept { return blocks_; }

BasicBlock::BasicBlock(std::string name) : name_(std::move(name)) {}
const std::string& BasicBlock::name() const noexcept { return name_; }
void BasicBlock::append(Instruction instruction) { instructions_.push_back(std::move(instruction)); }
std::vector<Instruction>& BasicBlock::instructions() noexcept { return instructions_; }
const std::vector<Instruction>& BasicBlock::instructions() const noexcept { return instructions_; }
void BasicBlock::eraseInstruction(std::size_t index) {
    if (index < instructions_.size()) instructions_.erase(instructions_.begin() + static_cast<std::ptrdiff_t>(index));
}

Instruction::Instruction(Opcode opcode) : opcode_(opcode) {}
Opcode Instruction::opcode() const noexcept { return opcode_; }
void Instruction::setOpcode(Opcode opcode) noexcept { opcode_ = opcode; }
void Instruction::addOperand(Value value) { operands_.push_back(std::move(value)); }
void Instruction::setOperands(std::vector<Value> values) { operands_ = std::move(values); }
const std::vector<Value>& Instruction::operands() const noexcept { return operands_; }
std::vector<Value>& Instruction::operands() noexcept { return operands_; }
void Instruction::setResult(Value value) { result_ = std::move(value); hasResult_ = true; }
void Instruction::clearResult() noexcept { result_ = {}; hasResult_ = false; }
bool Instruction::hasResult() const noexcept { return hasResult_; }
const Value& Instruction::result() const noexcept { return result_; }

const char* opcodeName(Opcode opcode) noexcept {
    switch (opcode) {
        case Opcode::Nop: return "nop"; case Opcode::Constant: return "constant";
        case Opcode::Move: return "move"; case Opcode::Copy: return "copy";
        case Opcode::Load: return "load"; case Opcode::Store: return "store";
        case Opcode::Add: return "add"; case Opcode::Subtract: return "sub";
        case Opcode::Multiply: return "mul"; case Opcode::Divide: return "div";
        case Opcode::Remainder: return "rem"; case Opcode::CompareEqual: return "cmpeq";
        case Opcode::CompareNotEqual: return "cmpne"; case Opcode::CompareLess: return "cmplt";
        case Opcode::CompareLessEqual: return "cmple"; case Opcode::CompareGreater: return "cmpgt";
        case Opcode::CompareGreaterEqual: return "cmpge"; case Opcode::Branch: return "br";
        case Opcode::ConditionalBranch: return "condbr"; case Opcode::Call: return "call";
        case Opcode::Return: return "return"; case Opcode::Allocate: return "alloc";
        case Opcode::Deallocate: return "dealloc"; case Opcode::Retain: return "retain";
        case Opcode::Release: return "release"; case Opcode::Phi: return "phi";
        case Opcode::Cast: return "cast"; case Opcode::Trap: return "trap";
    }
    return "unknown";
}

bool isTerminator(Opcode opcode) noexcept {
    return opcode == Opcode::Branch || opcode == Opcode::ConditionalBranch ||
           opcode == Opcode::Return || opcode == Opcode::Trap;
}

bool isMemoryOperation(Opcode opcode) noexcept {
    return opcode == Opcode::Load || opcode == Opcode::Store ||
           opcode == Opcode::Allocate || opcode == Opcode::Deallocate;
}

bool isPureOperation(Opcode opcode) noexcept {
    switch (opcode) {
        case Opcode::Constant: case Opcode::Move: case Opcode::Copy:
        case Opcode::Add: case Opcode::Subtract: case Opcode::Multiply:
        case Opcode::Divide: case Opcode::Remainder: case Opcode::CompareEqual:
        case Opcode::CompareNotEqual: case Opcode::CompareLess: case Opcode::CompareLessEqual:
        case Opcode::CompareGreater: case Opcode::CompareGreaterEqual: case Opcode::Cast:
            return true;
        default: return false;
    }
}

bool isOwnershipOperation(Opcode opcode) noexcept {
    return opcode == Opcode::Retain || opcode == Opcode::Release;
}

bool hasSideEffects(Opcode opcode) noexcept {
    return !isPureOperation(opcode) && opcode != Opcode::Nop && opcode != Opcode::Phi;
}

std::size_t instructionCount(const Module& module) noexcept {
    std::size_t count = 0;
    for (const auto& function : module.functions())
        for (const auto& block : function.blocks()) count += block.instructions().size();
    return count;
}

std::size_t basicBlockCount(const Module& module) noexcept {
    std::size_t count = 0;
    for (const auto& function : module.functions()) count += function.blocks().size();
    return count;
}

std::size_t functionCount(const Module& module) noexcept { return module.functions().size(); }
std::size_t operandCount(const Instruction& instruction) noexcept { return instruction.operands().size(); }

} // namespace hyper::hil
