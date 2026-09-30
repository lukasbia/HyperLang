#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
#include <cassert>
int main(){hyperlang::lexer::Lexer lexer("@import Foundation\n@include Math.hlf\nfunc main() { output.print(\"hello\") }");auto tokens=lexer.tokenize();assert(tokens.size()>10);assert(tokens[0].kind==hyperlang::lexer::TokenKind::AtImport);assert(tokens[2].kind==hyperlang::lexer::TokenKind::AtInclude);assert(tokens[6].kind==hyperlang::lexer::TokenKind::Func);assert(tokens.back().kind==hyperlang::lexer::TokenKind::EndOfFile);return 0;}
