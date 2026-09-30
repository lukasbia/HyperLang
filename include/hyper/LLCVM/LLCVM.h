#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace hyper::llcvm {

enum class Opcode : std::uint16_t {
    Invalid,
    Constant,
    Move,
    Load,
    Store,
    Add,
    Sub,
    Mul,
    Div,
    Compare,
    Branch,
    Call,
    Return,
    Retain,
    Release,
    Trap
};

struct SourceLocation {
    std::string file;
    std::size_t line = 0;
    std::size_t column = 0;
};

struct Instruction {
    Opcode opcode = Opcode::Invalid;
    std::vector<std::string> operands;
    SourceLocation location;
};

struct CompiledLine {
    std::size_t sourceLine = 0;
    std::string source;
    std::vector<Instruction> instructions;
};

class Module {
public:
    explicit Module(std::string name = {});
    void updateLine(CompiledLine line);
    const std::string& name() const noexcept;
    const std::vector<CompiledLine>& lines() const noexcept;

private:
    std::string name_;
    std::vector<CompiledLine> lines_;
};

} // namespace hyper::llcvm
