#pragma once
#include "hyperlang/HIL/HIL.h"
#include <cstddef>
#include <functional>
#include <string>
#include <vector>
namespace hyperlang::lsc {
struct IRInstruction { std::string opcode; std::string operand; };
struct IR { std::vector<IRInstruction> instructions; std::size_t revision = 0; };
struct MachineInstruction { std::string opcode; std::string operand; };
struct MachineCode { std::vector<MachineInstruction> instructions; };
struct CompilationSnapshot { std::size_t revision=0; hil::Module module; };
class LiveCompiler {
public:
 using UpdateCallback=std::function<void(const CompilationSnapshot&)>;
 void synchronize(const hil::Module& module);
 void setUpdateCallback(UpdateCallback callback);
 const CompilationSnapshot& snapshot() const;
private: CompilationSnapshot snapshot_{}; UpdateCallback callback_;
};
IR lower(const hil::Module& module);
MachineCode generateMachineCode(const IR& ir);
void emit(const MachineCode& machine);
}
