#include "hyper/Parser.h"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace hyper {

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), index_(0) {}

const Token& Parser::current() const {
    if (index_ >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[index_];
}

const Token& Parser::peek(std::size_t offset) const {
    const std::size_t position = index_ + offset;
    if (position >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[position];
}

bool Parser::isAtEnd() const {
    return current().kind == TokenKind::EndOfFile;
}

void Parser::advance() {
    if (!isAtEnd()) {
        ++index_;
    }
}

bool Parser::match(TokenKind kind) {
    if (current().kind != kind) {
        return false;
    }
    advance();
    return true;
}

bool Parser::expect(TokenKind kind) {
    if (match(kind)) {
        return true;
    }
    reportUnexpectedToken(current());
    return false;
}

void Parser::reportUnexpectedToken(const Token& token) {
    diagnostics_.push_back("unexpected token: " + token.text);
}

std::vector<Declaration> Parser::parse() {
    std::vector<Declaration> declarations;
    while (!isAtEnd()) {
        if (auto declaration = parseDeclaration()) {
            declarations.push_back(std::move(*declaration));
        } else {
            recoverDeclaration();
        }
    }
    return declarations;
}

std::optional<Declaration> Parser::parseDeclaration() {
    return parseFunctionDeclaration();
}

std::optional<Declaration> Parser::parseFunctionDeclaration() {
    if (!match(TokenKind::KeywordFunc)) {
        return std::nullopt;
    }

    Declaration declaration;
    declaration.kind = DeclarationKind::Function;

    if (current().kind != TokenKind::Identifier) {
        reportUnexpectedToken(current());
        return declaration;
    }

    declaration.name = current().text;
    advance();

    parseParameterList(declaration);
    parseFunctionBody(declaration);
    return declaration;
}

void Parser::parseParameterList(Declaration& declaration) {
    if (!match(TokenKind::LeftParen)) {
        return;
    }

    while (!isAtEnd() && current().kind != TokenKind::RightParen) {
        if (current().kind == TokenKind::Identifier) {
            declaration.parameters.push_back(current().text);
            advance();
        } else {
            reportUnexpectedToken(current());
            advance();
        }

        if (!match(TokenKind::Comma)) {
            break;
        }
    }

    expect(TokenKind::RightParen);
}

void Parser::parseFunctionBody(Declaration& declaration) {
    if (!match(TokenKind::LeftBrace)) {
        return;
    }

    declaration.body = parseStatementList();
    expect(TokenKind::RightBrace);
}

std::vector<Statement> Parser::parseStatementList() {
    std::vector<Statement> statements;
    while (!isAtEnd() && current().kind != TokenKind::RightBrace) {
        if (auto statement = parseStatement()) {
            statements.push_back(std::move(*statement));
        } else {
            recoverStatement();
        }
    }
    return statements;
}

std::optional<Statement> Parser::parseStatement() {
    if (current().kind == TokenKind::KeywordIf) {
        return parseIfStatement();
    }
    if (current().kind == TokenKind::KeywordWhile) {
        return parseWhileStatement();
    }
    return parseExpressionStatement();
}

std::optional<Statement> Parser::parseIfStatement() {
    Statement statement;
    statement.kind = StatementKind::If;
    expect(TokenKind::KeywordIf);
    statement.condition = parseExpression();
    if (match(TokenKind::LeftBrace)) {
        statement.body = parseStatementList();
        expect(TokenKind::RightBrace);
    }
    return statement;
}

std::optional<Statement> Parser::parseWhileStatement() {
    Statement statement;
    statement.kind = StatementKind::While;
    expect(TokenKind::KeywordWhile);
    statement.condition = parseExpression();
    if (match(TokenKind::LeftBrace)) {
        statement.body = parseStatementList();
        expect(TokenKind::RightBrace);
    }
    return statement;
}

std::optional<Statement> Parser::parseExpressionStatement() {
    auto expression = parseExpression();
    if (!expression) {
        return std::nullopt;
    }

    Statement statement;
    statement.kind = StatementKind::Expression;
    statement.expression = std::move(expression);
    match(TokenKind::Semicolon);
    return statement;
}

std::optional<Expression> Parser::parseExpression() {
    return parseBinaryExpression(0);
}

std::optional<Expression> Parser::parseBinaryExpression(int minimumPrecedence) {
    auto left = parsePrimaryExpression();
    if (!left) {
        return std::nullopt;
    }

    while (!isAtEnd()) {
        const int precedence = binaryPrecedence(current().kind);
        if (precedence < minimumPrecedence) {
            break;
        }

        const TokenKind operation = current().kind;
        advance();

        auto right = parseBinaryExpression(precedence + 1);
        if (!right) {
            return left;
        }

        Expression combined;
        combined.kind = ExpressionKind::Binary;
        combined.left = std::move(left);
        combined.right = std::move(right);
        combined.operatorKind = operation;
        left = std::move(combined);
    }

    return left;
}

std::optional<Expression> Parser::parsePrimaryExpression() {
    if (current().kind == TokenKind::Identifier) {
        Expression expression;
        expression.kind = ExpressionKind::Identifier;
        expression.text = current().text;
        advance();
        return expression;
    }

    if (current().kind == TokenKind::IntegerLiteral ||
        current().kind == TokenKind::FloatingLiteral ||
        current().kind == TokenKind::StringLiteral) {
        Expression expression;
        expression.kind = ExpressionKind::Literal;
        expression.text = current().text;
        advance();
        return expression;
    }

    if (match(TokenKind::LeftParen)) {
        auto expression = parseExpression();
        expect(TokenKind::RightParen);
        return expression;
    }

    return std::nullopt;
}

int Parser::binaryPrecedence(TokenKind kind) const {
    switch (kind) {
        case TokenKind::EqualEqual:
        case TokenKind::NotEqual:
        case TokenKind::GreaterEqual:
        case TokenKind::LessEqual:
            return 10;
        case TokenKind::Plus:
        case TokenKind::Minus:
            return 20;
        case TokenKind::Star:
        case TokenKind::Slash:
        case TokenKind::Percent:
            return 30;
        default:
            return -1;
    }
}

void Parser::recoverDeclaration() {
    while (!isAtEnd()) {
        if (current().kind == TokenKind::KeywordFunc ||
            current().kind == TokenKind::RightBrace) {
            return;
        }
        advance();
    }
}

void Parser::recoverStatement() {
    while (!isAtEnd()) {
        if (match(TokenKind::Semicolon) || current().kind == TokenKind::RightBrace) {
            return;
        }
        advance();
    }
}

} // namespace hyper
