#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace hyper::hil {

enum class Opcode {
    Nop,
    Constant,
    Move,
    Copy,
    Load,
    Store,
    Add,
    Subtract,
    Multiply,
    Divide,
    Remainder,
    CompareEqual,
    CompareNotEqual,
    CompareLess,
    CompareLessEqual,
    CompareGreater,
    CompareGreaterEqual,
    Branch,
    ConditionalBranch,
    Call,
    Return,
    Allocate,
    Deallocate,
    Retain,
    Release,
    Phi,
    Cast,
    Trap
};

struct Value {
    std::uint32_t id = 0;
    std::string type;
    std::string name;
};

class Instruction {
public:
    explicit Instruction(Opcode opcode);

    Opcode opcode() const noexcept;
    void addOperand(Value value);
    const std::vector<Value>& operands() const noexcept;

private:
    Opcode opcode_;
    std::vector<Value> operands_;
};

class BasicBlock {
public:
    explicit BasicBlock(std::string name);

    const std::string& name() const noexcept;
    void append(Instruction instruction);
    const std::vector<Instruction>& instructions() const noexcept;

private:
    std::string name_;
    std::vector<Instruction> instructions_;
};

class Function {
public:
    explicit Function(std::string name);

    const std::string& name() const noexcept;
    void addBlock(BasicBlock block);
    const std::vector<BasicBlock>& blocks() const noexcept;

private:
    std::string name_;
    std::vector<BasicBlock> blocks_;
};

class Module {
public:
    explicit Module(std::string name);

    const std::string& name() const noexcept;
    void addFunction(Function function);
    const std::vector<Function>& functions() const noexcept;

private:
    std::string name_;
    std::vector<Function> functions_;
};

} // namespace hyper::hil
