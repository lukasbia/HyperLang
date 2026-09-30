#include "hyperlang/Parser/Parser.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"

#include <memory>
#include <utility>
#include <vector>

namespace hyperlang::parser {

Parser::Parser(std::vector<lexer::Token> tokens)
    : tokens_(std::move(tokens)) {
}

const lexer::Token &Parser::current() const {
    return tokens_[index_];
}

bool Parser::match(lexer::TokenKind kind) {
    if (current().kind != kind) {
        return false;
    }

    ++index_;
    return true;
}

std::unique_ptr<ast::Function> Parser::parseFunction() {
    if (!match(lexer::TokenKind::Func)) {
        return nullptr;
    }

    if (current().kind != lexer::TokenKind::Identifier) {
        return nullptr;
    }

    auto function = std::make_unique<ast::Function>(current().text);
    ++index_;

    if (!match(lexer::TokenKind::LParen)) {
        return function;
    }

    while (current().kind != lexer::TokenKind::RParen &&
           current().kind != lexer::TokenKind::EndOfFile) {
        ++index_;
    }

    match(lexer::TokenKind::RParen);

    if (!match(lexer::TokenKind::LBrace)) {
        return function;
    }

    int braceDepth = 1;

    while (braceDepth > 0 &&
           current().kind != lexer::TokenKind::EndOfFile) {
        if (match(lexer::TokenKind::LBrace)) {
            ++braceDepth;
            continue;
        }

        if (match(lexer::TokenKind::RBrace)) {
            --braceDepth;
            continue;
        }

        ++index_;
    }

    return function;
}

std::unique_ptr<ast::Program> Parser::parse() {
    auto program = std::make_unique<ast::Program>();

    while (current().kind != lexer::TokenKind::EndOfFile) {
        auto function = parseFunction();

        if (function != nullptr) {
            program->functions.push_back(std::move(function));
            continue;
        }

        ++index_;
    }

    return program;
}

} // namespace hyperlang::parser
