#include "hyper/lib/Parse/Lexer.h"
#include <utility>
namespace hyper::parse {
void Lexer::error(std::string message){errorAt(makeToken(TokenKind::Unknown,tokenStart_).range,std::move(message));}
void Lexer::errorAt(SourceRange range,std::string message){
 LexerDiagnostic d{DiagnosticSeverity::Error,range,std::move(message)};diagnostics_.push_back(d);if(diagnosticHandler_)diagnosticHandler_(diagnostics_.back());
}
}
