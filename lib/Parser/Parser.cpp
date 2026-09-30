#include "hyperlang/Parser/Parser.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::parser {
Parser::Parser(std::vector<lexer::Token> t):tokens_(std::move(t)){}
const lexer::Token& Parser::current()const{return tokens_[index_];}
bool Parser::match(lexer::TokenKind k){if(current().kind!=k)return false;++index_;return true;}
std::unique_ptr<ast::Function> Parser::parseFunction(){if(!match(lexer::TokenKind::Func))return {};if(current().kind!=lexer::TokenKind::Identifier)return {};auto f=std::make_unique<ast::Function>(current().text);++index_;if(!match(lexer::TokenKind::LParen))return f;while(current().kind!=lexer::TokenKind::RParen&&current().kind!=lexer::TokenKind::EndOfFile)++index_;match(lexer::TokenKind::RParen);if(match(lexer::TokenKind::LBrace)){int depth=1;while(depth&&current().kind!=lexer::TokenKind::EndOfFile){if(match(lexer::TokenKind::LBrace))++depth;else if(match(lexer::TokenKind::RBrace))--depth;else ++index_;}}return f;}
std::unique_ptr<ast::Program> Parser::parse(){auto p=std::make_unique<ast::Program>();while(current().kind!=lexer::TokenKind::EndOfFile){auto f=parseFunction();if(f)p->functions.push_back(std::move(f));else ++index_;}return p;}
}
