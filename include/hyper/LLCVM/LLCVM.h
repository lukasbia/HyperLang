#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace hyper::llcvm {

enum class Opcode : std::uint16_t {
    Invalid, Constant, Move, Load, Store, Add, Sub, Mul, Div, Compare,
    Branch, Call, Return, Retain, Release, Trap
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
    std::uint64_t generation = 0;
};

class Module {
public:
    explicit Module(std::string name = {});
    bool updateLine(CompiledLine line);
    bool removeLine(std::size_t sourceLine);
    const CompiledLine* findLine(std::size_t sourceLine) const noexcept;
    const std::string& name() const noexcept;
    const std::vector<CompiledLine>& lines() const noexcept;
    std::uint64_t generation() const noexcept;

private:
    std::string name_;
    std::vector<CompiledLine> lines_;
    std::uint64_t generation_ = 0;
};

const char* opcodeName(Opcode) noexcept;
bool isTerminator(Opcode) noexcept;
bool isPure(Opcode) noexcept;

struct SourceSnapshot {
    std::vector<std::string> lines;
    std::uint64_t revision = 0;
};

class SourceTracker {
public:
    void replace(SourceSnapshot snapshot);
    const SourceSnapshot& snapshot() const noexcept;
    std::vector<std::size_t> changedLines(const SourceSnapshot& next) const;
private:
    SourceSnapshot snapshot_;
};

class LineChangeDetector {
public:
    static std::vector<std::size_t> detect(const SourceSnapshot& before,
                                            const SourceSnapshot& after);
};

class IncrementalScheduler {
public:
    void schedule(std::size_t line);
    bool empty() const noexcept;
    std::size_t next();
private:
    std::vector<std::size_t> pending_;
};

class CodeCache {
public:
    void store(std::size_t line, CompiledLine compiled);
    const CompiledLine* lookup(std::size_t line) const noexcept;
    void erase(std::size_t line);
    void clear();
private:
    std::unordered_map<std::size_t, CompiledLine> entries_;
};

class MachineCodeCache {
public:
    void store(std::size_t line, std::vector<std::uint8_t> bytes);
    const std::vector<std::uint8_t>* lookup(std::size_t line) const noexcept;
    void erase(std::size_t line);
private:
    std::unordered_map<std::size_t, std::vector<std::uint8_t>> entries_;
};

struct Diagnostic {
    SourceLocation location;
    std::string message;
    bool error = true;
};

class LiveDiagnostics {
public:
    void clearLine(std::size_t line);
    void add(Diagnostic diagnostic);
    const std::vector<Diagnostic>& diagnostics() const noexcept;
private:
    std::vector<Diagnostic> diagnostics_;
};

std::vector<std::size_t> compileChangedLines(Module& module,
                                              SourceTracker& tracker,
                                              const SourceSnapshot& next);
std::vector<std::uint8_t> encodeInstruction(const Instruction& instruction);
bool verifyLine(const CompiledLine& line, std::string* error = nullptr);

} // namespace hyper::llcvm
