#include "hyper/HIL/HIL.h"
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace hyper::hil {

Module::Module(std::string name) : name_(std::move(name)) {}

const std::string& Module::name() const noexcept { return name_; }

void Module::addFunction(Function function) {
    functions_.push_back(std::move(function));
}

const std::vector<Function>& Module::functions() const noexcept {
    return functions_;
}

Function::Function(std::string name) : name_(std::move(name)) {}

const std::string& Function::name() const noexcept { return name_; }

void Function::addBlock(BasicBlock block) {
    blocks_.push_back(std::move(block));
}

const std::vector<BasicBlock>& Function::blocks() const noexcept {
    return blocks_;
}

BasicBlock::BasicBlock(std::string name) : name_(std::move(name)) {}

const std::string& BasicBlock::name() const noexcept { return name_; }

void BasicBlock::append(Instruction instruction) {
    instructions_.push_back(std::move(instruction));
}

const std::vector<Instruction>& BasicBlock::instructions() const noexcept {
    return instructions_;
}

Instruction::Instruction(Opcode opcode) : opcode_(opcode) {}

Opcode Instruction::opcode() const noexcept { return opcode_; }

void Instruction::addOperand(Value value) {
    operands_.push_back(std::move(value));
}

const std::vector<Value>& Instruction::operands() const noexcept {
    return operands_;
}

} // namespace hyper::hil
