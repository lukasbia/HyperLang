#include "hyperlang/Diagnostics/DiagnosticEngine.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::diagnostics {
void Engine::report(Severity severity,std::string message){diagnostics_.push_back({severity,std::move(message)});}
const std::vector<Diagnostic>& Engine::diagnostics()const{return diagnostics_;}
bool Engine::hasErrors()const{for(const auto& d:diagnostics_)if(d.severity==Severity::Error||d.severity==Severity::Fatal)return true;return false;}
}
