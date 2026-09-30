#pragma once
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include <vector>
namespace hyperlang::parser {
class Parser {
public: explicit Parser(std::vector<lexer::Token> tokens); std::unique_ptr<ast::Program> parse();
private: std::vector<lexer::Token> tokens_; std::size_t index_=0; const lexer::Token& current() const; bool match(lexer::TokenKind); std::unique_ptr<ast::Function> parseFunction();
};
}
