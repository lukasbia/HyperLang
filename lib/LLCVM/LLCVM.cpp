#include "hyper/LLCVM/LLCVM.h"
#include <algorithm>
#include <utility>

namespace hyper::llcvm {

Module::Module(std::string name) : name_(std::move(name)) {}

bool Module::updateLine(CompiledLine line) {
    line.generation = ++generation_;
    auto it = std::lower_bound(lines_.begin(), lines_.end(), line.sourceLine,
        [](const CompiledLine& current, std::size_t number) { return current.sourceLine < number; });
    if (it != lines_.end() && it->sourceLine == line.sourceLine) {
        *it = std::move(line);
        return false;
    }
    lines_.insert(it, std::move(line));
    return true;
}

bool Module::removeLine(std::size_t sourceLine) {
    auto it = std::lower_bound(lines_.begin(), lines_.end(), sourceLine,
        [](const CompiledLine& current, std::size_t number) { return current.sourceLine < number; });
    if (it == lines_.end() || it->sourceLine != sourceLine) return false;
    lines_.erase(it);
    ++generation_;
    return true;
}

const CompiledLine* Module::findLine(std::size_t sourceLine) const noexcept {
    auto it = std::lower_bound(lines_.begin(), lines_.end(), sourceLine,
        [](const CompiledLine& current, std::size_t number) { return current.sourceLine < number; });
    return it != lines_.end() && it->sourceLine == sourceLine ? &*it : nullptr;
}

const std::string& Module::name() const noexcept { return name_; }
const std::vector<CompiledLine>& Module::lines() const noexcept { return lines_; }
std::uint64_t Module::generation() const noexcept { return generation_; }

const char* opcodeName(Opcode opcode) noexcept {
    switch (opcode) {
        case Opcode::Constant: return "constant"; case Opcode::Move: return "move";
        case Opcode::Load: return "load"; case Opcode::Store: return "store";
        case Opcode::Add: return "add"; case Opcode::Sub: return "sub";
        case Opcode::Mul: return "mul"; case Opcode::Div: return "div";
        case Opcode::Compare: return "compare"; case Opcode::Branch: return "branch";
        case Opcode::Call: return "call"; case Opcode::Return: return "return";
        case Opcode::Retain: return "retain"; case Opcode::Release: return "release";
        case Opcode::Trap: return "trap"; case Opcode::Invalid: return "invalid";
    }
    return "invalid";
}

bool isTerminator(Opcode opcode) noexcept {
    return opcode == Opcode::Branch || opcode == Opcode::Return || opcode == Opcode::Trap;
}

bool isPure(Opcode opcode) noexcept {
    return opcode == Opcode::Constant || opcode == Opcode::Move || opcode == Opcode::Add ||
           opcode == Opcode::Sub || opcode == Opcode::Mul || opcode == Opcode::Div || opcode == Opcode::Compare;
}

} // namespace hyper::llcvm
