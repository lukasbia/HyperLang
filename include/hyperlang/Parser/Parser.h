#pragma once
#include "hyperlang/AST/AST.h"
#include "hyperlang/Lexer/Token.h"
#include <memory>
#include <vector>
namespace hyperlang::parser {
class Parser {
public:
    explicit Parser(std::vector<lexer::Token> tokens);
    std::unique_ptr<ast::Program> parse();
private:
    std::vector<lexer::Token> tokens_;
    std::size_t index_ = 0;
    const lexer::Token& current() const;
    const lexer::Token& previous() const;
    bool check(lexer::TokenKind kind) const;
    bool match(lexer::TokenKind kind);
    bool consume(lexer::TokenKind kind);
    std::unique_ptr<ast::Function> parseFunction();
    void parseFunctionBody(ast::Function& function);
};
}
