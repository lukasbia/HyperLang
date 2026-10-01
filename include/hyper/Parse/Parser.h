#pragma once

#include "hyper/Parse/Lexer.h"
#include "hyper/Parse/ParserDiagnostics.h"
#include "hyper/Parse/ParserOptions.h"
#include "hyper/Parse/Syntax.h"
#include "hyper/Parse/ParserCore.h"

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace hyper {

class Parser {
public:
    explicit Parser(std::string_view source);
    explicit Parser(std::vector<Token> tokens);
    ~Parser() = default;

    Parser(Parser&&) noexcept = default;
    Parser& operator=(Parser&&) noexcept = default;
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::unique_ptr<SyntaxNode> parse();
    std::unique_ptr<SyntaxNode> parseTranslationUnit();

    const std::vector<ParseDiagnostic>& diagnostics() const noexcept;
    void clearDiagnostics();

    void setOptions(const ParserOptions& options);
    const ParserOptions& options() const noexcept;

    bool atEnd() const noexcept;
    const Token& current() const;
    const Token& previous() const;
    const Token& peek(std::size_t distance = 0) const;

    bool check(TokenKind kind) const noexcept;
    bool checkAny(std::initializer_list<TokenKind> kinds) const noexcept;
    bool consume(TokenKind kind);
    bool consumeAny(std::initializer_list<TokenKind> kinds);
    const Token& advance();
    bool expect(TokenKind kind, std::string_view message);

    void diagnose(SourceLocation location, ParseDiagnostic::Severity severity,
                  std::string message, std::string fixIt = {});
    void synchronize();
    void synchronizeStatement();
    void synchronizeDeclaration();

    std::unique_ptr<SyntaxNode> parseDeclaration();
    std::unique_ptr<SyntaxNode> parseImportDeclaration();
    std::unique_ptr<SyntaxNode> parseFunctionDeclaration();
    std::unique_ptr<SyntaxNode> parseVariableDeclaration();
    std::unique_ptr<SyntaxNode> parseTypeDeclaration();
    std::unique_ptr<SyntaxNode> parseStructDeclaration();
    std::unique_ptr<SyntaxNode> parseClassDeclaration();
    std::unique_ptr<SyntaxNode> parseEnumDeclaration();
    std::unique_ptr<SyntaxNode> parseProtocolDeclaration();
    std::unique_ptr<SyntaxNode> parseExtensionDeclaration();

    std::unique_ptr<SyntaxNode> parseStatement();
    std::unique_ptr<SyntaxNode> parseCompoundStatement();
    std::unique_ptr<SyntaxNode> parseIfStatement();
    std::unique_ptr<SyntaxNode> parseWhileStatement();
    std::unique_ptr<SyntaxNode> parseForStatement();
    std::unique_ptr<SyntaxNode> parseDoStatement();
    std::unique_ptr<SyntaxNode> parseGuardStatement();
    std::unique_ptr<SyntaxNode> parseSwitchStatement();
    std::unique_ptr<SyntaxNode> parseReturnStatement();
    std::unique_ptr<SyntaxNode> parseThrowStatement();
    std::unique_ptr<SyntaxNode> parseDeferStatement();
    std::unique_ptr<SyntaxNode> parseExpressionStatement();

    std::unique_ptr<SyntaxNode> parseExpression();
    std::unique_ptr<SyntaxNode> parseAssignmentExpression();
    std::unique_ptr<SyntaxNode> parseBinaryExpression(int minimumPrecedence);
    std::unique_ptr<SyntaxNode> parsePrefixExpression();
    std::unique_ptr<SyntaxNode> parsePostfixExpression();
    std::unique_ptr<SyntaxNode> parsePrimaryExpression();
    std::unique_ptr<SyntaxNode> parseIdentifierExpression();
    std::unique_ptr<SyntaxNode> parseLiteralExpression();
    std::unique_ptr<SyntaxNode> parseCallExpression(std::unique_ptr<SyntaxNode> base);
    std::unique_ptr<SyntaxNode> parseMemberExpression(std::unique_ptr<SyntaxNode> base);
    std::unique_ptr<SyntaxNode> parseSubscriptExpression(std::unique_ptr<SyntaxNode> base);
    std::unique_ptr<SyntaxNode> parseClosureExpression();
    std::unique_ptr<SyntaxNode> parseTupleExpression();
    std::unique_ptr<SyntaxNode> parseArrayExpression();
    std::unique_ptr<SyntaxNode> parseDictionaryExpression();

    std::unique_ptr<SyntaxNode> parseType();
    std::unique_ptr<SyntaxNode> parseNamedType();
    std::unique_ptr<SyntaxNode> parseGenericType();
    std::unique_ptr<SyntaxNode> parseFunctionType();
    std::unique_ptr<SyntaxNode> parseTupleType();
    std::unique_ptr<SyntaxNode> parseTypeAnnotation();
    std::unique_ptr<SyntaxNode> parseParameterClause();
    std::unique_ptr<SyntaxNode> parseParameter();
    std::unique_ptr<SyntaxNode> parseGenericParameterClause();
    std::unique_ptr<SyntaxNode> parseAttribute();
    std::unique_ptr<SyntaxNode> parseModifier();

    static bool isDeclarationStart(TokenKind kind) noexcept;
    static bool isStatementStart(TokenKind kind) noexcept;
    static bool isLiteral(TokenKind kind) noexcept;
    static bool isTypeStart(TokenKind kind) noexcept;
    static bool isAssignmentOperator(TokenKind kind) noexcept;
    static int precedence(TokenKind kind) noexcept;
    static SyntaxKind syntaxForToken(TokenKind kind) noexcept;
    static std::string tokenText(const Token& token);

private:
    std::vector<Token> tokens_;
    std::size_t index_ = 0;
    ParserOptions options_{};
    std::vector<ParseDiagnostic> diagnostics_;
};

} // namespace hyper
