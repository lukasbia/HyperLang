#include "hyper/Parser.h"
#include "hyper/ParserDeclaration.h"
#include "hyper/ParserDiagnostics.h"
#include "hyper/ParserExpression.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hyper {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)), index_(0) {}
Parser::Parser(std::string_view source) : tokens_(), index_(0) { (void)source; }

const Token& Parser::current() const { return peek(); }

const Token& Parser::previous() const {
    if (index_ == 0 || tokens_.empty()) return current();
    return tokens_[index_ - 1];
}

const Token& Parser::peek(std::size_t offset) const {
    static const Token emptyToken{};
    const std::size_t position = index_ + offset;
    if (position >= tokens_.size()) return tokens_.empty() ? emptyToken : tokens_.back();
    return tokens_[position];
}

bool Parser::check(TokenKind kind) const { return current().kind == kind; }

bool Parser::checkAny(std::initializer_list<TokenKind> kinds) const {
    for (TokenKind kind : kinds) if (check(kind)) return true;
    return false;
}

bool Parser::consume(TokenKind kind) {
    if (!check(kind)) return false;
    advance();
    return true;
}

bool Parser::consumeAny(std::initializer_list<TokenKind> kinds) {
    if (!checkAny(kinds)) return false;
    advance();
    return true;
}

const Token& Parser::advance() {
    const Token& token = current();
    if (!atEnd()) ++index_;
    return token;
}

bool Parser::expect(TokenKind kind, std::string_view message) {
    if (consume(kind)) return true;
    diagnose(current().location, ParseDiagnostic::Severity::Error, std::string(message));
    return false;
}

void Parser::diagnose(SourceLocation location, ParseDiagnostic::Severity severity,
                      std::string message, std::string fixIt) {
    diagnostics_.push_back(ParseDiagnostic{severity, location, std::move(message), std::move(fixIt)});
}

bool Parser::atEnd() const noexcept {
    return tokens_.empty() || current().kind == TokenKind::EndOfFile;
}

std::unique_ptr<SyntaxNode> Parser::parse() { return parseTranslationUnit(); }

std::unique_ptr<SyntaxNode> Parser::parseTranslationUnit() {
    auto unit = std::make_unique<SyntaxNode>(SyntaxKind::TranslationUnit);
    while (!atEnd()) {
        const std::size_t before = index_;
        if (auto declaration = parseDeclaration()) unit->children.push_back(std::move(declaration));
        else if (auto statement = parseStatement()) unit->children.push_back(std::move(statement));
        else synchronize();
        if (before == index_ && !atEnd()) advance();
    }
    return unit;
}

std::unique_ptr<SyntaxNode> Parser::parseDeclaration() {
    if (check(TokenKind::KeywordFunc)) return parseFunctionDeclaration();
    if (check(TokenKind::KeywordImport)) return parseImportDeclaration();
    if (checkAny({TokenKind::KeywordVar, TokenKind::KeywordLet})) return parseVariableDeclaration();
    if (check(TokenKind::KeywordType)) return parseTypeDeclaration();
    return nullptr;
}

std::unique_ptr<SyntaxNode> Parser::parseImportDeclaration() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::ImportDeclaration);
    node->text = tokenText(advance());
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseFunctionDeclaration() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::FunctionDeclaration);
    node->text = tokenText(advance());
    if (check(TokenKind::Identifier)) node->children.push_back(parseIdentifierExpression());
    else diagnose(current().location, ParseDiagnostic::Severity::Error, "expected function name");
    if (check(TokenKind::LeftParen)) node->children.push_back(parseParameterClause());
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseVariableDeclaration() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::VariableDeclaration);
    node->text = tokenText(advance());
    if (check(TokenKind::Identifier)) node->children.push_back(parseIdentifierExpression());
    if (check(TokenKind::Colon)) node->children.push_back(parseTypeAnnotation());
    if (check(TokenKind::Equal)) node->children.push_back(parseAssignmentExpression());
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseTypeDeclaration() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::TypeDeclaration);
    node->text = tokenText(advance());
    if (check(TokenKind::Identifier)) node->children.push_back(parseIdentifierExpression());
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseStructDeclaration() { return std::make_unique<SyntaxNode>(SyntaxKind::StructDeclaration); }
std::unique_ptr<SyntaxNode> Parser::parseClassDeclaration() { return std::make_unique<SyntaxNode>(SyntaxKind::ClassDeclaration); }
std::unique_ptr<SyntaxNode> Parser::parseEnumDeclaration() { return std::make_unique<SyntaxNode>(SyntaxKind::EnumDeclaration); }
std::unique_ptr<SyntaxNode> Parser::parseProtocolDeclaration() { return std::make_unique<SyntaxNode>(SyntaxKind::ProtocolDeclaration); }
std::unique_ptr<SyntaxNode> Parser::parseExtensionDeclaration() { return std::make_unique<SyntaxNode>(SyntaxKind::ExtensionDeclaration); }

std::unique_ptr<SyntaxNode> Parser::parseStatement() {
    if (check(TokenKind::KeywordIf)) return parseIfStatement();
    if (check(TokenKind::KeywordWhile)) return parseWhileStatement();
    if (check(TokenKind::KeywordFor)) return parseForStatement();
    if (check(TokenKind::KeywordDo)) return parseDoStatement();
    if (check(TokenKind::KeywordGuard)) return parseGuardStatement();
    if (check(TokenKind::KeywordSwitch)) return parseSwitchStatement();
    if (check(TokenKind::KeywordReturn)) return parseReturnStatement();
    if (check(TokenKind::KeywordThrow)) return parseThrowStatement();
    if (check(TokenKind::KeywordDefer)) return parseDeferStatement();
    if (check(TokenKind::LeftBrace)) return parseCompoundStatement();
    return parseExpressionStatement();
}

std::unique_ptr<SyntaxNode> Parser::parseCompoundStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::CompoundStatement);
    consume(TokenKind::LeftBrace);
    while (!atEnd() && !check(TokenKind::RightBrace)) {
        const std::size_t before = index_;
        if (auto statement = parseStatement()) node->children.push_back(std::move(statement));
        else synchronizeStatement();
        if (before == index_ && !atEnd()) advance();
    }
    expect(TokenKind::RightBrace, "expected '}'");
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseIfStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::IfStatement); advance();
    if (auto condition = parseExpression()) node->children.push_back(std::move(condition));
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseWhileStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::WhileStatement); advance();
    if (auto condition = parseExpression()) node->children.push_back(std::move(condition));
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseForStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::ForStatement); advance();
    if (auto expression = parseExpression()) node->children.push_back(std::move(expression));
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseDoStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::DoStatement); advance();
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseGuardStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::GuardStatement); advance();
    if (auto expression = parseExpression()) node->children.push_back(std::move(expression));
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseSwitchStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::SwitchStatement); advance();
    if (auto expression = parseExpression()) node->children.push_back(std::move(expression));
    if (check(TokenKind::LeftBrace)) node->children.push_back(parseCompoundStatement());
    return node;
}
std::unique_ptr<SyntaxNode> Parser::parseReturnStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::ReturnStatement); advance();
    if (!check(TokenKind::Semicolon) && !check(TokenKind::RightBrace) && !atEnd()) {
        if (auto expression = parseExpression()) node->children.push_back(std::move(expression));
    }
    consume(TokenKind::Semicolon); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseThrowStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::ThrowStatement); advance();
    if (auto expression = parseExpression()) node->children.push_back(std::move(expression)); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseDeferStatement() {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::DeferStatement); advance();
    if (auto statement = parseStatement()) node->children.push_back(std::move(statement)); return node;
}

std::unique_ptr<SyntaxNode> Parser::parseExpressionStatement() {
    auto expression = parseExpression(); if (!expression) return nullptr;
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::ExpressionStatement);
    node->children.push_back(std::move(expression)); consume(TokenKind::Semicolon); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseExpression() { return parseAssignmentExpression(); }
std::unique_ptr<SyntaxNode> Parser::parseAssignmentExpression() {
    auto left = parseBinaryExpression(); if (!left) return nullptr;
    if (isAssignmentOperator(current().kind)) {
        auto node = std::make_unique<SyntaxNode>(SyntaxKind::AssignmentExpression);
        node->children.push_back(std::move(left)); node->text = tokenText(advance());
        if (auto right = parseAssignmentExpression()) node->children.push_back(std::move(right));
        return node;
    }
    return left;
}
std::unique_ptr<SyntaxNode> Parser::parseBinaryExpression(int minimumPrecedence) {
    auto left = parsePrefixExpression(); if (!left) return nullptr;
    while (!atEnd()) {
        const int currentPrecedence = precedence(current().kind);
        if (currentPrecedence < minimumPrecedence) break;
        auto node = std::make_unique<SyntaxNode>(SyntaxKind::BinaryExpression);
        node->children.push_back(std::move(left)); node->text = tokenText(advance());
        if (auto right = parseBinaryExpression(currentPrecedence + 1)) node->children.push_back(std::move(right));
        left = std::move(node);
    }
    return left;
}
std::unique_ptr<SyntaxNode> Parser::parsePrefixExpression() {
    if (checkAny({TokenKind::Bang, TokenKind::Minus, TokenKind::Plus})) {
        auto node = std::make_unique<SyntaxNode>(SyntaxKind::PrefixExpression);
        node->text = tokenText(advance());
        if (auto operand = parsePrefixExpression()) node->children.push_back(std::move(operand)); return node;
    }
    return parsePostfixExpression();
}
std::unique_ptr<SyntaxNode> Parser::parsePostfixExpression() {
    auto base = parsePrimaryExpression(); if (!base) return nullptr;
    while (!atEnd()) {
        if (check(TokenKind::LeftParen)) base = parseCallExpression(std::move(base));
        else if (check(TokenKind::Dot)) base = parseMemberExpression(std::move(base));
        else if (check(TokenKind::LeftBracket)) base = parseSubscriptExpression(std::move(base));
        else break;
    }
    return base;
}
std::unique_ptr<SyntaxNode> Parser::parsePrimaryExpression() {
    if (isLiteral(current().kind)) return parseLiteralExpression();
    if (check(TokenKind::Identifier)) return parseIdentifierExpression();
    if (check(TokenKind::LeftParen)) return parseTupleExpression();
    if (check(TokenKind::LeftBracket)) return parseArrayExpression();
    return nullptr;
}
std::unique_ptr<SyntaxNode> Parser::parseIdentifierExpression() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::IdentifierExpression); node->text = tokenText(advance()); return node; }
std::unique_ptr<SyntaxNode> Parser::parseLiteralExpression() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::LiteralExpression); node->text = tokenText(advance()); return node; }
std::unique_ptr<SyntaxNode> Parser::parseCallExpression(std::unique_ptr<SyntaxNode> base) {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::CallExpression); node->children.push_back(std::move(base)); consume(TokenKind::LeftParen);
    while (!atEnd() && !check(TokenKind::RightParen)) { if (auto argument = parseExpression()) node->children.push_back(std::move(argument)); else break; if (!consume(TokenKind::Comma)) break; }
    expect(TokenKind::RightParen, "expected ')' after arguments"); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseMemberExpression(std::unique_ptr<SyntaxNode> base) {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::MemberExpression); node->children.push_back(std::move(base)); consume(TokenKind::Dot); if (check(TokenKind::Identifier)) node->children.push_back(parseIdentifierExpression()); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseSubscriptExpression(std::unique_ptr<SyntaxNode> base) {
    auto node = std::make_unique<SyntaxNode>(SyntaxKind::SubscriptExpression); node->children.push_back(std::move(base)); consume(TokenKind::LeftBracket); if (auto index = parseExpression()) node->children.push_back(std::move(index)); expect(TokenKind::RightBracket, "expected ']'"); return node;
}
std::unique_ptr<SyntaxNode> Parser::parseClosureExpression() { return std::make_unique<SyntaxNode>(SyntaxKind::ClosureExpression); }
std::unique_ptr<SyntaxNode> Parser::parseTupleExpression() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::TupleExpression); consume(TokenKind::LeftParen); while (!atEnd() && !check(TokenKind::RightParen)) { if (auto e = parseExpression()) node->children.push_back(std::move(e)); else break; if (!consume(TokenKind::Comma)) break; } expect(TokenKind::RightParen, "expected ')'"); return node; }
std::unique_ptr<SyntaxNode> Parser::parseArrayExpression() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::ArrayExpression); consume(TokenKind::LeftBracket); while (!atEnd() && !check(TokenKind::RightBracket)) { if (auto e = parseExpression()) node->children.push_back(std::move(e)); else break; if (!consume(TokenKind::Comma)) break; } expect(TokenKind::RightBracket, "expected ']'"); return node; }
std::unique_ptr<SyntaxNode> Parser::parseDictionaryExpression() { return std::make_unique<SyntaxNode>(SyntaxKind::DictionaryExpression); }
std::unique_ptr<SyntaxNode> Parser::parseType() { return parseNamedType(); }
std::unique_ptr<SyntaxNode> Parser::parseNamedType() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::NamedType); if (check(TokenKind::Identifier)) node->text = tokenText(advance()); return node; }
std::unique_ptr<SyntaxNode> Parser::parseGenericType() { return std::make_unique<SyntaxNode>(SyntaxKind::GenericType); }
std::unique_ptr<SyntaxNode> Parser::parseFunctionType() { return std::make_unique<SyntaxNode>(SyntaxKind::FunctionType); }
std::unique_ptr<SyntaxNode> Parser::parseTupleType() { return std::make_unique<SyntaxNode>(SyntaxKind::TupleType); }
std::unique_ptr<SyntaxNode> Parser::parseTypeAnnotation() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::TypeAnnotation); consume(TokenKind::Colon); if (auto type = parseType()) node->children.push_back(std::move(type)); return node; }
std::unique_ptr<SyntaxNode> Parser::parseParameterClause() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::ParameterClause); consume(TokenKind::LeftParen); while (!atEnd() && !check(TokenKind::RightParen)) { if (auto p = parseParameter()) node->children.push_back(std::move(p)); else break; if (!consume(TokenKind::Comma)) break; } expect(TokenKind::RightParen, "expected ')' after parameters"); return node; }
std::unique_ptr<SyntaxNode> Parser::parseParameter() { auto node = std::make_unique<SyntaxNode>(SyntaxKind::Parameter); if (check(TokenKind::Identifier)) node->children.push_back(parseIdentifierExpression()); if (check(TokenKind::Colon)) node->children.push_back(parseTypeAnnotation()); return node; }
std::unique_ptr<SyntaxNode> Parser::parseGenericParameterClause() { return std::make_unique<SyntaxNode>(SyntaxKind::GenericParameterClause); }
std::unique_ptr<SyntaxNode> Parser::parseAttribute() { return std::make_unique<SyntaxNode>(SyntaxKind::Attribute); }
std::unique_ptr<SyntaxNode> Parser::parseModifier() { return std::make_unique<SyntaxNode>(SyntaxKind::Modifier); }

void Parser::synchronize() { synchronizeDeclaration(); }
void Parser::synchronizeStatement() { while (!atEnd() && !checkAny({TokenKind::Semicolon, TokenKind::RightBrace})) advance(); consume(TokenKind::Semicolon); }
void Parser::synchronizeDeclaration() { while (!atEnd() && !checkAny({TokenKind::KeywordFunc, TokenKind::KeywordImport, TokenKind::KeywordVar, TokenKind::KeywordLet})) advance(); }
const std::vector<ParseDiagnostic>& Parser::diagnostics() const noexcept { return diagnostics_; }
void Parser::clearDiagnostics() { diagnostics_.clear(); }
void Parser::setOptions(const ParserOptions& options) { options_ = options; }
const ParserOptions& Parser::options() const noexcept { return options_; }
bool Parser::isDeclarationStart(TokenKind kind) noexcept { return kind == TokenKind::KeywordFunc || kind == TokenKind::KeywordImport || kind == TokenKind::KeywordVar || kind == TokenKind::KeywordLet; }
bool Parser::isStatementStart(TokenKind kind) noexcept { return kind == TokenKind::KeywordIf || kind == TokenKind::KeywordWhile || kind == TokenKind::KeywordFor || kind == TokenKind::Identifier; }
bool Parser::isLiteral(TokenKind kind) noexcept { return kind == TokenKind::IntegerLiteral || kind == TokenKind::FloatingLiteral || kind == TokenKind::StringLiteral; }
bool Parser::isTypeStart(TokenKind kind) noexcept { return kind == TokenKind::Identifier; }
bool Parser::isAssignmentOperator(TokenKind kind) noexcept { return kind == TokenKind::Equal || kind == TokenKind::PlusEqual || kind == TokenKind::MinusEqual || kind == TokenKind::StarEqual; }
int Parser::precedence(TokenKind kind) noexcept { switch (kind) { case TokenKind::EqualEqual: case TokenKind::NotEqual: case TokenKind::GreaterEqual: case TokenKind::LessEqual: return 10; case TokenKind::Plus: case TokenKind::Minus: return 20; case TokenKind::Star: case TokenKind::Slash: case TokenKind::Percent: return 30; default: return -1; } }
SyntaxKind Parser::syntaxForToken(TokenKind) noexcept { return SyntaxKind::Unknown; }
std::string Parser::tokenText(const Token& token) { return token.text; }
const char* syntaxKindName(SyntaxKind) noexcept { return "Syntax"; }

} // namespace hyper
