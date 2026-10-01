#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "hyper/Parse/Parser.h"
#include "hyper/HIL/HIL.h"
#include "hyper/HILOptimization/HILOptimization.h"
#include "hyper/LLCVM/LLCVM.h"
#include "hyper/LLCVMBackend/Backend.h"
namespace hyper::compiler {
enum class Stage { Parse, Sema, HIL, HILOptimization, LLCVM, LLCVMOptimization, Backend, Complete };
struct Diagnostic { Stage stage=Stage::Parse; std::string message; bool error=true; };
struct Options { llcvm::backend::TargetArchitecture target=llcvm::backend::TargetArchitecture::X86_64; bool optimize=true; };
struct Result {
 bool success=false; std::vector<Diagnostic> diagnostics; hil::Module hilModule{"main"};
 llcvm::Module llcvmModule{"main"}; llcvm::backend::MachineCode machineCode{};
};
class Compiler {
public:
 explicit Compiler(Options options={});
 Result compile(std::string_view source,std::string_view fileName={});
private:
 Options options_;
};
} // namespace hyper::compiler
