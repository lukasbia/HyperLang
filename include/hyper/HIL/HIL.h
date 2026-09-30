#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace hyper::hil {

enum class Opcode {
    Nop, Constant, Move, Copy, Load, Store, Add, Subtract, Multiply, Divide,
    Remainder, CompareEqual, CompareNotEqual, CompareLess, CompareLessEqual,
    CompareGreater, CompareGreaterEqual, Branch, ConditionalBranch, Call, Return,
    Allocate, Deallocate, Retain, Release, Phi, Cast, Trap
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
    void setOpcode(Opcode opcode) noexcept;
    void addOperand(Value value);
    void setOperands(std::vector<Value> values);
    const std::vector<Value>& operands() const noexcept;
    std::vector<Value>& operands() noexcept;
    void setResult(Value value);
    void clearResult() noexcept;
    bool hasResult() const noexcept;
    const Value& result() const noexcept;
private:
    Opcode opcode_;
    std::vector<Value> operands_;
    Value result_{};
    bool hasResult_ = false;
};

class BasicBlock {
public:
    explicit BasicBlock(std::string name);
    const std::string& name() const noexcept;
    void append(Instruction instruction);
    std::vector<Instruction>& instructions() noexcept;
    const std::vector<Instruction>& instructions() const noexcept;
    void eraseInstruction(std::size_t index);
private:
    std::string name_;
    std::vector<Instruction> instructions_;
};

class Function {
public:
    explicit Function(std::string name);
    const std::string& name() const noexcept;
    void addBlock(BasicBlock block);
    std::vector<BasicBlock>& blocks() noexcept;
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
    std::vector<Function>& functions() noexcept;
    const std::vector<Function>& functions() const noexcept;
private:
    std::string name_;
    std::vector<Function> functions_;
};

const char* opcodeName(Opcode) noexcept;
bool isTerminator(Opcode) noexcept;
bool isMemoryOperation(Opcode) noexcept;
bool isPureOperation(Opcode) noexcept;
bool isOwnershipOperation(Opcode) noexcept;
bool hasSideEffects(Opcode) noexcept;
std::size_t instructionCount(const Module&) noexcept;
std::size_t basicBlockCount(const Module&) noexcept;
std::size_t functionCount(const Module&) noexcept;
std::size_t operandCount(const Instruction&) noexcept;

} // namespace hyper::hil
