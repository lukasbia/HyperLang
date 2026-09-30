#pragma once
#include "hyperlang/AST/AST.h"
#include <string>
#include <vector>
namespace hyperlang::hil {
enum class Opcode { FunctionBegin, FunctionEnd, LoadLiteral, LoadVariable, StoreVariable, Retain, Release, Borrow, Move, Call, Return, Branch, BranchIf, Add, Subtract, Multiply, Divide, Compare };
struct Instruction { Opcode opcode; std::string operand; };
struct Function { std::string name; std::vector<Instruction> instructions; };
struct Module { std::vector<Function> functions; };
const char* opcodeName(Opcode opcode);
Module lower(const ast::Program& program);
void optimize(Module& module);
} // namespace hyperlang::hil
