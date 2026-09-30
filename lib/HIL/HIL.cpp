#include "hyperlang/HIL/HIL.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/Parser/Parser.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::hil {
const char* opcodeName(Opcode op){switch(op){case Opcode::FunctionBegin:return "function_begin";case Opcode::FunctionEnd:return "function_end";case Opcode::LoadLiteral:return "load_literal";case Opcode::LoadVariable:return "load_variable";case Opcode::StoreVariable:return "store_variable";case Opcode::Call:return "call";case Opcode::Return:return "return";case Opcode::Branch:return "branch";case Opcode::BranchIf:return "branch_if";case Opcode::Add:return "add";case Opcode::Subtract:return "subtract";case Opcode::Multiply:return "multiply";case Opcode::Divide:return "divide";case Opcode::Compare:return "compare";}return "unknown";}
}
