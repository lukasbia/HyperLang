#include "hyperlang/Lexer/Token.h"
#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::lexer {
const char* tokenKindName(TokenKind k) {
 switch(k) {
 case TokenKind::EndOfFile:return "eof"; case TokenKind::Identifier:return "identifier"; case TokenKind::IntegerLiteral:return "integer";
 case TokenKind::NumberLiteral:return "number"; case TokenKind::StringLiteral:return "string"; case TokenKind::AtInclude:return "@include"; case TokenKind::AtImport:return "@import";
 case TokenKind::LBrace:return "{"; case TokenKind::RBrace:return "}"; case TokenKind::LParen:return "("; case TokenKind::RParen:return ")"; case TokenKind::Dot:return ".";
 case TokenKind::Equal:return "="; case TokenKind::EqualEqual:return "=="; case TokenKind::NotEqual:return "!="; case TokenKind::Plus:return "+"; case TokenKind::Minus:return "-";
 case TokenKind::Star:return "*"; case TokenKind::Slash:return "/"; case TokenKind::Percent:return "%"; case TokenKind::Less:return "<"; case TokenKind::LessEqual:return "<=";
 case TokenKind::Greater:return ">"; case TokenKind::GreaterEqual:return ">="; case TokenKind::AndAnd:return "&&"; case TokenKind::OrOr:return "||"; case TokenKind::Bang:return "!";
 default:return "keyword-or-token";
 }
}
}
