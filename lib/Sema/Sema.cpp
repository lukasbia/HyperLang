#include "hyperlang/Sema/Sema.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::sema {
bool Analyzer::analyze(const ast::Program& program){diagnostics_.clear();for(const auto& f:program.functions){if(f->name.empty())diagnostics_.push_back({"function name cannot be empty"});}return diagnostics_.empty();}
const std::vector<Diagnostic>& Analyzer::diagnostics()const{return diagnostics_;}
}
