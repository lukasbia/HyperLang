#include "hyperlang/HIL/HIL.h"
#include "hyperlang/HIL/Optimization/HILOptimizer.h"
#include <utility>
namespace hyperlang::hil {
const char* opcodeName(Opcode op) {
    switch (op) {
    case Opcode::FunctionBegin: return "function_begin";
    case Opcode::FunctionEnd: return "function_end";
    case Opcode::LoadLiteral: return "load_literal";
    case Opcode::LoadVariable: return "load_variable";
    case Opcode::StoreVariable: return "store_variable";
    case Opcode::Retain: return "retain";
    case Opcode::Release: return "release";
    case Opcode::Borrow: return "borrow";
    case Opcode::Move: return "move";
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
    for (const auto& sourceFunction : program.functions) {
        Function function;
        function.name = sourceFunction->name;
        function.instructions.push_back({Opcode::FunctionBegin, function.name});
        for (const auto& statement : sourceFunction->body) {
            if (dynamic_cast<const ast::Return*>(statement.get()))
                function.instructions.push_back({Opcode::Return, {}});
            else
                function.instructions.push_back({Opcode::Call, "statement"});
        }
        function.instructions.push_back({Opcode::FunctionEnd, function.name});
        module.functions.push_back(std::move(function));
    }
    return module;
}
void optimize(Module& module) { runOptimization(module); }
} // namespace hyperlang::hil
