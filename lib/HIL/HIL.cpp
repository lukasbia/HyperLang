#include "hyperlang/HIL/HIL.h"
#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include "hyperlang/AST/AST.h"
namespace hyperlang::hil {
const char* opcodeName(Opcode op) {
    switch (op) {
        case Opcode::FunctionBegin: return "function_begin";
        case Opcode::FunctionEnd: return "function_end";
        case Opcode::LoadLiteral: return "load_literal";
        case Opcode::LoadVariable: return "load_variable";
        case Opcode::StoreVariable: return "store_variable";
        case Opcode::Call: return "call";
        case Opcode::Return: return "return";
        case Opcode::Branch: return "branch";
        case Opcode::BranchIf: return "branch_if";
        case Opcode::Add: return "add";
        case Opcode::Subtract: return "subtract";
        case Opcode::Multiply: return "multiply";
        case Opcode::Divide: return "divide";
        case Opcode::Compare: return "compare";
    }
    return "unknown";
}
Module lower(const ast::Program& program) {
    Module module;
    for (const auto& function : program.functions) {
        Function result;
        result.name = function->name;
        result.instructions.push_back({Opcode::FunctionBegin, result.name});
        result.instructions.push_back({Opcode::FunctionEnd, result.name});
        module.functions.push_back(std::move(result));
    }
    return module;
}
void optimize(Module& module) { hil::optimize(module); }
}
