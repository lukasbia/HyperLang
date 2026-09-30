#include "hyperlang/Parser/Parser.h"
#include <utility>
namespace hyperlang::parser {
Parser::Parser(std::vector<lexer::Token> tokens) : tokens_(std::move(tokens)) {}
const lexer::Token& Parser::current() const { return tokens_[index_]; }
const lexer::Token& Parser::previous() const { return tokens_[index_ - 1]; }
bool Parser::check(lexer::TokenKind kind) const { return current().kind == kind; }
bool Parser::match(lexer::TokenKind kind) { if (!check(kind)) return false; ++index_; return true; }
bool Parser::consume(lexer::TokenKind kind) { return match(kind); }
std::unique_ptr<ast::Function> Parser::parseFunction() {
    if (!match(lexer::TokenKind::Func) || !check(lexer::TokenKind::Identifier)) return nullptr;
    auto function = std::make_unique<ast::Function>(current().text); ++index_;
    if (match(lexer::TokenKind::LParen)) {
        while (!check(lexer::TokenKind::RParen) && !check(lexer::TokenKind::EndOfFile)) {
            if (check(lexer::TokenKind::Identifier)) function->parameters.push_back(current().text);
            ++index_;
        }
        consume(lexer::TokenKind::RParen);
    }
    parseFunctionBody(*function);
    return function;
}
void Parser::parseFunctionBody(ast::Function& function) {
    if (!consume(lexer::TokenKind::LBrace)) return;
    int depth = 1;
    while (depth > 0 && !check(lexer::TokenKind::EndOfFile)) {
        if (match(lexer::TokenKind::LBrace)) { ++depth; continue; }
        if (match(lexer::TokenKind::RBrace)) { --depth; continue; }
        if (match(lexer::TokenKind::Return)) {
            function.body.push_back(std::make_unique<ast::Return>());
            while (!check(lexer::TokenKind::Semicolon) && !check(lexer::TokenKind::RBrace) && !check(lexer::TokenKind::EndOfFile)) ++index_;
            match(lexer::TokenKind::Semicolon);
            continue;
        }
        ++index_;
    }
}
std::unique_ptr<ast::Program> Parser::parse() {
    auto program = std::make_unique<ast::Program>();
    while (!check(lexer::TokenKind::EndOfFile)) {
        if (auto function = parseFunction()) { program->functions.push_back(std::move(function)); continue; }
        ++index_;
    }
    return program;
}
} // namespace hyperlang::parser
