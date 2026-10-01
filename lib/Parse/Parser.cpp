#include "hyper/Parse/Parser.h"
#include "hyper/Parse/ParserDiagnostics.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hyper {

namespace {

static Token makeEOFToken() {
    Token token;
    token.kind = TokenKind::EndOfFile;
    return token;
}

static std::string_view safeTokenText(const Token& token) {
    return token.text;
}

static std::unique_ptr<SyntaxNode> makeNode(SyntaxKind kind) {
    return std::make_unique<SyntaxNode>(kind);
}

static void appendChild(
    SyntaxNode& parent,
    std::unique_ptr<SyntaxNode> child
) {
    if (child) {
        parent.children.push_back(
            std::move(child)
        );
    }
}

static bool isOpeningDelimiter(TokenKind kind) {
    switch (kind) {
        case TokenKind::LeftParen:
        case TokenKind::LeftBrace:
        case TokenKind::LeftBracket:
            return true;
        default:
            return false;
    }
}

static bool isClosingDelimiter(TokenKind kind) {
    switch (kind) {
        case TokenKind::RightParen:
        case TokenKind::RightBrace:
        case TokenKind::RightBracket:
            return true;
        default:
            return false;
    }
}

static TokenKind matchingClose(TokenKind kind) {
    switch (kind) {
        case TokenKind::LeftParen:
            return TokenKind::RightParen;
        case TokenKind::LeftBrace:
            return TokenKind::RightBrace;
        case TokenKind::LeftBracket:
            return TokenKind::RightBracket;
        default:
            return TokenKind::EndOfFile;
    }
}

static bool isModifierToken(TokenKind kind) {
    switch (kind) {
        case TokenKind::KwPublic:
        case TokenKind::KwPrivate:
        case TokenKind::KwInternal:
        case TokenKind::KwProtected:
        case TokenKind::KwStatic:
        case TokenKind::KwFinal:
        case TokenKind::KwOverride:
        case TokenKind::KwMutating:
        case TokenKind::KwAsync:
        case TokenKind::KwDetached:
        case TokenKind::KwIsolated:
        case TokenKind::KwNonisolated:
        case TokenKind::KwSendable:
        case TokenKind::KwExtern:
            return true;
        default:
            return false;
    }
}

static bool isBuiltinTypeToken(TokenKind kind) {
    switch (kind) {
        case TokenKind::KwInt:
        case TokenKind::KwFloat:
        case TokenKind::KwBool:
        case TokenKind::KwString:
        case TokenKind::KwBytes:
        case TokenKind::KwNum:
        case TokenKind::KwAny:
        case TokenKind::KwSome:
            return true;
        default:
            return false;
    }
}

static bool isPrefixToken(TokenKind kind) {
    switch (kind) {
        case TokenKind::Plus:
        case TokenKind::Minus:
        case TokenKind::Bang:
        case TokenKind::Tilde:
        case TokenKind::Ampersand:
        case TokenKind::KwNot:
        case TokenKind::KwMove:
        case TokenKind::KwBorrow:
        case TokenKind::KwConsume:
        case TokenKind::KwCopy:
            return true;
        default:
            return false;
    }
}

static bool isPostfixToken(TokenKind kind) {
    switch (kind) {
        case TokenKind::Question:
        case TokenKind::Bang:
            return true;
        default:
            return false;
    }
}

static bool isBinaryToken(TokenKind kind) {
    switch (kind) {
        case TokenKind::Plus:
        case TokenKind::Minus:
        case TokenKind::Star:
        case TokenKind::Slash:
        case TokenKind::Percent:
        case TokenKind::Ampersand:
        case TokenKind::Pipe:
        case TokenKind::Caret:
        case TokenKind::Tilde:
        case TokenKind::EqualEqual:
        case TokenKind::NotEqual:
        case TokenKind::Greater:
        case TokenKind::GreaterEqual:
        case TokenKind::Less:
        case TokenKind::LessEqual:
        case TokenKind::AndAnd:
        case TokenKind::OrOr:
        case TokenKind::QuestionQuestion:
        case TokenKind::TildeEqual:
        case TokenKind::Range:
        case TokenKind::ClosedRange:
        case TokenKind::Power:
        case TokenKind::ShiftLeft:
        case TokenKind::ShiftRight:
            return true;
        default:
            return false;
    }
}

} // namespace

Parser::Parser(
    std::string_view source
) {
    Lexer lexer(
        source
    );

    tokens_ = lexer.tokenize();

    if (tokens_.empty()) {
        tokens_.push_back(
            makeEOFToken()
        );
    }
}

Parser::Parser(
    std::vector<Token> tokens
) : tokens_(
    std::move(tokens)
) {
    if (tokens_.empty()) {
        tokens_.push_back(
            makeEOFToken()
        );
    }
}

std::unique_ptr<SyntaxNode> Parser::parse() {
    return parseTranslationUnit();
}

std::unique_ptr<SyntaxNode> Parser::parseTranslationUnit() {
    auto root = makeNode(
        SyntaxKind::TranslationUnit
    );

    while (!atEnd()) {
        const std::size_t before = index_;

        auto declaration = parseDeclaration();

        if (declaration) {
            appendChild(
                *root,
                std::move(declaration)
            );
        }

        if (index_ == before) {
            diagnose(
                current().location,
                ParseDiagnostic::Severity::Error,
                "parser made no progress"
            );
            advance();
        }
    }

    return root;
}

const std::vector<ParseDiagnostic>& Parser::diagnostics() const noexcept {
    return diagnostics_;
}

void Parser::clearDiagnostics() {
    diagnostics_.clear();
}

void Parser::setOptions(
    const ParserOptions& options
) {
    options_ = options;
}

const ParserOptions& Parser::options() const noexcept {
    return options_;
}

bool Parser::atEnd() const noexcept {
    return current().kind == TokenKind::EndOfFile;
}

const Token& Parser::current() const {
    return peek(
        0
    );
}

const Token& Parser::previous() const {
    if (index_ == 0) {
        return current();
    }

    return tokens_[
        index_ - 1
    ];
}

const Token& Parser::peek(
    std::size_t distance
) const {
    const std::size_t position = index_ + distance;

    if (position >= tokens_.size()) {
        static const Token eof = makeEOFToken();
        return eof;
    }

    return tokens_[
        position
    ];
}

bool Parser::check(
    TokenKind kind
) const {
    return current().kind == kind;
}

bool Parser::checkAny(
    std::initializer_list<TokenKind> kinds
) const {
    for (const TokenKind kind : kinds) {
        if (check(kind)) {
            return true;
        }
    }

    return false;
}

bool Parser::consume(
    TokenKind kind
) {
    if (!check(kind)) {
        return false;
    }

    advance();
    return true;
}

bool Parser::consumeAny(
    std::initializer_list<TokenKind> kinds
) {
    for (const TokenKind kind : kinds) {
        if (consume(kind)) {
            return true;
        }
    }

    return false;
}

const Token& Parser::advance() {
    const Token& token = current();

    if (!atEnd()) {
        ++index_;
    }

    return token;
}

bool Parser::expect(
    TokenKind kind,
    std::string_view message
) {
    if (consume(kind)) {
        return true;
    }

    diagnose(
        current().location,
        ParseDiagnostic::Severity::Error,
        std::string(message)
    );

    return false;
}

void Parser::diagnose(
    SourceLocation location,
    ParseDiagnostic::Severity severity,
    std::string message,
    std::string fixIt
) {
    diagnostics_.push_back(
        ParseDiagnostic{
            severity,
            location,
            std::move(message),
            std::move(fixIt)
        }
    );
}

void Parser::synchronize() {
    while (!atEnd()) {
        if (isDeclarationStart(current().kind)) {
            return;
        }

        if (isStatementStart(current().kind)) {
            return;
        }

        if (current().kind == TokenKind::RightBrace) {
            return;
        }

        advance();
    }
}

void Parser::synchronizeStatement() {
    while (!atEnd()) {
        if (consume(TokenKind::Semicolon)) {
            return;
        }

        if (check(TokenKind::RightBrace)) {
            return;
        }

        if (isStatementStart(current().kind)) {
            return;
        }

        advance();
    }
}

void Parser::synchronizeDeclaration() {
    while (!atEnd()) {
        if (isDeclarationStart(current().kind)) {
            return;
        }

        if (check(TokenKind::RightBrace)) {
            return;
        }

        advance();
    }
}

std::unique_ptr<SyntaxNode> Parser::parseDeclaration() {
    if (check(TokenKind::KwImport)) {
        return parseImportDeclaration();
    }

    if (check(TokenKind::KwFunc)) {
        return parseFunctionDeclaration();
    }

    if (checkAny({
        TokenKind::KwVar,
        TokenKind::KwLet,
        TokenKind::KwConst
    })) {
        return parseVariableDeclaration();
    }

    if (check(TokenKind::KwType)) {
        return parseTypeDeclaration();
    }

    if (check(TokenKind::KwStruct)) {
        return parseStructDeclaration();
    }

    if (check(TokenKind::KwClass)) {
        return parseClassDeclaration();
    }

    if (check(TokenKind::KwEnum)) {
        return parseEnumDeclaration();
    }

    if (check(TokenKind::KwProtocol)) {
        return parseProtocolDeclaration();
    }

    if (check(TokenKind::KwExtension)) {
        return parseExtensionDeclaration();
    }

    if (options_.allowTopLevelCode) {
        return parseStatement();
    }

    diagnose(
        current().location,
        ParseDiagnostic::Severity::Error,
        "expected a declaration"
    );

    synchronizeDeclaration();
    return nullptr;
}

std::unique_ptr<SyntaxNode> Parser::parseImportDeclaration() {
    auto node = makeNode(
        SyntaxKind::ImportDeclaration
    );

    const Token& keyword = advance();
    node->location.begin = keyword.location;

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected module name after import"
        );
    }

    while (consume(TokenKind::Dot)) {
        if (!check(TokenKind::Identifier)) {
            diagnose(
                current().location,
                ParseDiagnostic::Severity::Error,
                "expected identifier after import separator"
            );
            break;
        }

        auto part = makeNode(
            SyntaxKind::IdentifierExpression
        );

        part->text = tokenText(
            advance()
        );

        appendChild(
            *node,
            std::move(part)
        );
    }

    node->location.end = previous().location;

    consume(TokenKind::Semicolon);
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseFunctionDeclaration() {
    auto node = makeNode(
        SyntaxKind::FunctionDeclaration
    );

    const Token& keyword = advance();
    node->location.begin = keyword.location;

    while (isModifierToken(current().kind)) {
        appendChild(
            *node,
            parseModifier()
        );
    }

    if (!check(TokenKind::Identifier)) {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected function name"
        );
        synchronizeDeclaration();
        return node;
    }

    node->text = tokenText(
        advance()
    );

    if (options_.parseGenerics && check(TokenKind::Less)) {
        appendChild(
            *node,
            parseGenericParameterClause()
        );
    }

    if (check(TokenKind::LeftParen)) {
        appendChild(
            *node,
            parseParameterClause()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected function parameter clause"
        );
    }

    if (consume(TokenKind::Arrow)) {
        auto returnClause = makeNode(
            SyntaxKind::ReturnClause
        );

        appendChild(
            *returnClause,
            parseType()
        );

        appendChild(
            *node,
            std::move(returnClause)
        );
    }

    while (options_.parseAttributes && check(TokenKind::AtSign)) {
        appendChild(
            *node,
            parseAttribute()
        );
    }

    if (check(TokenKind::LeftBrace)) {
        appendChild(
            *node,
            parseCompoundStatement()
        );
    } else {
        consume(TokenKind::Semicolon);
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseVariableDeclaration() {
    auto node = makeNode(
        SyntaxKind::VariableDeclaration
    );

    node->location.begin = current().location;

    advance();

    if (!check(TokenKind::Identifier)) {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected variable name"
        );
        synchronizeStatement();
        return node;
    }

    node->text = tokenText(
        advance()
    );

    if (consume(TokenKind::Colon)) {
        appendChild(
            *node,
            parseTypeAnnotation()
        );
    }

    if (consume(TokenKind::Equal)) {
        auto initializer = makeNode(
            SyntaxKind::ExpressionStatement
        );

        appendChild(
            *initializer,
            parseExpression()
        );

        appendChild(
            *node,
            std::move(initializer)
        );
    }

    consume(TokenKind::Semicolon);

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseTypeDeclaration() {
    auto node = makeNode(
        SyntaxKind::TypeDeclaration
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected type name"
        );
    }

    if (consume(TokenKind::Equal)) {
        appendChild(
            *node,
            parseType()
        );
    }

    consume(TokenKind::Semicolon);
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseStructDeclaration() {
    auto node = makeNode(
        SyntaxKind::StructDeclaration
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    }

    if (options_.parseGenerics && check(TokenKind::Less)) {
        appendChild(
            *node,
            parseGenericParameterClause()
        );
    }

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            if (auto member = parseDeclaration()) {
                appendChild(
                    *node,
                    std::move(member)
                );
            } else {
                synchronizeDeclaration();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for struct"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseClassDeclaration() {
    auto node = makeNode(
        SyntaxKind::ClassDeclaration
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    }

    if (options_.parseGenerics && check(TokenKind::Less)) {
        appendChild(
            *node,
            parseGenericParameterClause()
        );
    }

    if (consume(TokenKind::Colon)) {
        while (isTypeStart(current().kind)) {
            appendChild(
                *node,
                parseType()
            );

            if (!consume(TokenKind::Comma)) {
                break;
            }
        }
    }

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            auto member = parseDeclaration();

            if (member) {
                appendChild(
                    *node,
                    std::move(member)
                );
            } else {
                synchronizeDeclaration();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for class"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseEnumDeclaration() {
    auto node = makeNode(
        SyntaxKind::EnumDeclaration
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    }

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            auto member = makeNode(
                SyntaxKind::CaseStatement
            );

            if (consume(TokenKind::KwCase)) {
                if (check(TokenKind::Identifier)) {
                    member->text = tokenText(
                        advance()
                    );
                }

                appendChild(
                    *node,
                    std::move(member)
                );
            } else {
                synchronizeDeclaration();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for enum"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseProtocolDeclaration() {
    auto node = makeNode(
        SyntaxKind::ProtocolDeclaration
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    }

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            auto requirement = parseDeclaration();

            if (requirement) {
                appendChild(
                    *node,
                    std::move(requirement)
                );
            } else {
                synchronizeDeclaration();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for protocol"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseExtensionDeclaration() {
    auto node = makeNode(
        SyntaxKind::ExtensionDeclaration
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseType()
    );

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            auto member = parseDeclaration();

            if (member) {
                appendChild(
                    *node,
                    std::move(member)
                );
            } else {
                synchronizeDeclaration();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for extension"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseStatement() {
    if (check(TokenKind::LeftBrace)) {
        return parseCompoundStatement();
    }

    if (check(TokenKind::KwIf)) {
        return parseIfStatement();
    }

    if (check(TokenKind::KwWhile)) {
        return parseWhileStatement();
    }

    if (check(TokenKind::KwFor)) {
        return parseForStatement();
    }

    if (check(TokenKind::KwDo)) {
        return parseDoStatement();
    }

    if (check(TokenKind::KwGuard)) {
        return parseGuardStatement();
    }

    if (check(TokenKind::KwSwitch)) {
        return parseSwitchStatement();
    }

    if (check(TokenKind::KwReturn)) {
        return parseReturnStatement();
    }

    if (check(TokenKind::KwThrow)) {
        return parseThrowStatement();
    }

    if (check(TokenKind::KwDefer)) {
        return parseDeferStatement();
    }

    if (checkAny({
        TokenKind::KwVar,
        TokenKind::KwLet,
        TokenKind::KwConst
    })) {
        return parseVariableDeclaration();
    }

    return parseExpressionStatement();
}

std::unique_ptr<SyntaxNode> Parser::parseCompoundStatement() {
    auto node = makeNode(
        SyntaxKind::CompoundStatement
    );

    node->location.begin = current().location;

    expect(
        TokenKind::LeftBrace,
        "expected opening brace"
    );

    while (!atEnd() && !check(TokenKind::RightBrace)) {
        const std::size_t before = index_;
        auto statement = parseStatement();

        if (statement) {
            appendChild(
                *node,
                std::move(statement)
            );
        }

        if (index_ == before) {
            synchronizeStatement();
        }
    }

    expect(
        TokenKind::RightBrace,
        "expected closing brace"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseIfStatement() {
    auto node = makeNode(
        SyntaxKind::IfStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseExpression()
    );

    appendChild(
        *node,
        parseCompoundStatement()
    );

    if (consume(TokenKind::KwElse)) {
        if (check(TokenKind::KwIf)) {
            appendChild(
                *node,
                parseIfStatement()
            );
        } else {
            appendChild(
                *node,
                parseCompoundStatement()
            );
        }
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseWhileStatement() {
    auto node = makeNode(
        SyntaxKind::WhileStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseExpression()
    );

    appendChild(
        *node,
        parseCompoundStatement()
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseForStatement() {
    auto node = makeNode(
        SyntaxKind::ForStatement
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        auto binding = makeNode(
            SyntaxKind::IdentifierExpression
        );

        binding->text = tokenText(
            advance()
        );

        appendChild(
            *node,
            std::move(binding)
        );
    }

    if (consume(TokenKind::KwIn)) {
        appendChild(
            *node,
            parseExpression()
        );
    }

    appendChild(
        *node,
        parseCompoundStatement()
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseDoStatement() {
    auto node = makeNode(
        SyntaxKind::DoStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseCompoundStatement()
    );

    if (consume(TokenKind::KwCatch)) {
        appendChild(
            *node,
            parseCompoundStatement()
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseGuardStatement() {
    auto node = makeNode(
        SyntaxKind::GuardStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseExpression()
    );

    if (consume(TokenKind::KwElse)) {
        appendChild(
            *node,
            parseCompoundStatement()
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseSwitchStatement() {
    auto node = makeNode(
        SyntaxKind::SwitchStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseExpression()
    );

    if (consume(TokenKind::LeftBrace)) {
        while (!atEnd() && !check(TokenKind::RightBrace)) {
            if (check(TokenKind::KwCase) || check(TokenKind::KwDefault)) {
                auto caseNode = makeNode(
                    SyntaxKind::CaseStatement
                );

                caseNode->location.begin = current().location;
                advance();

                if (!check(TokenKind::Colon)) {
                    appendChild(
                        *caseNode,
                        parseExpression()
                    );
                }

                consume(TokenKind::Colon);

                while (
                    !atEnd()
                    && !check(TokenKind::KwCase)
                    && !check(TokenKind::KwDefault)
                    && !check(TokenKind::RightBrace)
                ) {
                    appendChild(
                        *caseNode,
                        parseStatement()
                    );
                }

                caseNode->location.end = previous().location;

                appendChild(
                    *node,
                    std::move(caseNode)
                );
            } else {
                synchronizeStatement();
            }
        }

        expect(
            TokenKind::RightBrace,
            "expected closing brace for switch"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseReturnStatement() {
    auto node = makeNode(
        SyntaxKind::ReturnStatement
    );

    node->location.begin = current().location;
    advance();

    if (!check(TokenKind::Semicolon) && !check(TokenKind::RightBrace)) {
        appendChild(
            *node,
            parseExpression()
        );
    }

    consume(TokenKind::Semicolon);
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseThrowStatement() {
    auto node = makeNode(
        SyntaxKind::ThrowStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseExpression()
    );

    consume(TokenKind::Semicolon);
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseDeferStatement() {
    auto node = makeNode(
        SyntaxKind::DeferStatement
    );

    node->location.begin = current().location;
    advance();

    appendChild(
        *node,
        parseStatement()
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseExpressionStatement() {
    auto node = makeNode(
        SyntaxKind::ExpressionStatement
    );

    node->location.begin = current().location;

    appendChild(
        *node,
        parseExpression()
    );

    consume(TokenKind::Semicolon);

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseExpression() {
    return parseAssignmentExpression();
}

std::unique_ptr<SyntaxNode> Parser::parseAssignmentExpression() {
    auto left = parseBinaryExpression();

    if (!left) {
        return nullptr;
    }

    if (!isAssignmentOperator(current().kind)) {
        return left;
    }

    auto node = makeNode(
        SyntaxKind::AssignmentExpression
    );

    node->location.begin = left->location.begin;
    node->text = tokenText(
        advance()
    );

    appendChild(
        *node,
        std::move(left)
    );

    appendChild(
        *node,
        parseAssignmentExpression()
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseBinaryExpression(
    int minimumPrecedence
) {
    auto left = parsePrefixExpression();

    if (!left) {
        return nullptr;
    }

    while (isBinaryToken(current().kind)) {
        const int currentPrecedence = precedence(
            current().kind
        );

        if (currentPrecedence < minimumPrecedence) {
            break;
        }

        const Token operatorToken = current();
        advance();

        auto right = parseBinaryExpression(
            currentPrecedence + 1
        );

        if (!right) {
            diagnose(
                current().location,
                ParseDiagnostic::Severity::Error,
                "expected expression after binary operator"
            );
            break;
        }

        auto node = makeNode(
            SyntaxKind::BinaryExpression
        );

        node->location.begin = left->location.begin;
        node->text = tokenText(
            operatorToken
        );

        appendChild(
            *node,
            std::move(left)
        );

        appendChild(
            *node,
            std::move(right)
        );

        node->location.end = previous().location;
        left = std::move(node);
    }

    return left;
}

std::unique_ptr<SyntaxNode> Parser::parsePrefixExpression() {
    if (isPrefixToken(current().kind)) {
        auto node = makeNode(
            SyntaxKind::PrefixExpression
        );

        node->location.begin = current().location;
        node->text = tokenText(
            advance()
        );

        appendChild(
            *node,
            parsePrefixExpression()
        );

        node->location.end = previous().location;
        return node;
    }

    return parsePostfixExpression();
}

std::unique_ptr<SyntaxNode> Parser::parsePostfixExpression() {
    auto expression = parsePrimaryExpression();

    if (!expression) {
        return nullptr;
    }

    while (true) {
        if (check(TokenKind::LeftParen)) {
            expression = parseCallExpression(
                std::move(expression)
            );
            continue;
        }

        if (check(TokenKind::Dot)) {
            expression = parseMemberExpression(
                std::move(expression)
            );
            continue;
        }

        if (check(TokenKind::LeftBracket)) {
            expression = parseSubscriptExpression(
                std::move(expression)
            );
            continue;
        }

        if (isPostfixToken(current().kind)) {
            auto node = makeNode(
                SyntaxKind::PostfixExpression
            );

            node->location.begin = expression->location.begin;
            node->text = tokenText(
                advance()
            );

            appendChild(
                *node,
                std::move(expression)
            );

            node->location.end = previous().location;
            expression = std::move(node);
            continue;
        }

        break;
    }

    return expression;
}

std::unique_ptr<SyntaxNode> Parser::parsePrimaryExpression() {
    if (check(TokenKind::Identifier)) {
        return parseIdentifierExpression();
    }

    if (isLiteral(current().kind)) {
        return parseLiteralExpression();
    }

    if (check(TokenKind::LeftParen)) {
        return parseTupleExpression();
    }

    if (check(TokenKind::LeftBracket)) {
        return parseArrayExpression();
    }

    if (check(TokenKind::LeftBrace)) {
        return parseClosureExpression();
    }

    if (isBuiltinTypeToken(current().kind)) {
        auto node = makeNode(
            SyntaxKind::IdentifierExpression
        );

        node->text = tokenText(
            advance()
        );

        return node;
    }

    diagnose(
        current().location,
        ParseDiagnostic::Severity::Error,
        "expected expression"
    );

    return nullptr;
}

std::unique_ptr<SyntaxNode> Parser::parseIdentifierExpression() {
    auto node = makeNode(
        SyntaxKind::IdentifierExpression
    );

    node->location.begin = current().location;
    node->text = tokenText(
        advance()
    );
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseLiteralExpression() {
    auto node = makeNode(
        SyntaxKind::LiteralExpression
    );

    node->location.begin = current().location;
    node->text = tokenText(
        advance()
    );
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseCallExpression(
    std::unique_ptr<SyntaxNode> base
) {
    auto node = makeNode(
        SyntaxKind::CallExpression
    );

    node->location.begin = base->location.begin;

    appendChild(
        *node,
        std::move(base)
    );

    expect(
        TokenKind::LeftParen,
        "expected opening parenthesis for call"
    );

    while (!atEnd() && !check(TokenKind::RightParen)) {
        appendChild(
            *node,
            parseExpression()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightParen,
        "expected closing parenthesis for call"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseMemberExpression(
    std::unique_ptr<SyntaxNode> base
) {
    auto node = makeNode(
        SyntaxKind::MemberExpression
    );

    node->location.begin = base->location.begin;

    appendChild(
        *node,
        std::move(base)
    );

    consume(TokenKind::Dot);

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected member name after dot"
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseSubscriptExpression(
    std::unique_ptr<SyntaxNode> base
) {
    auto node = makeNode(
        SyntaxKind::SubscriptExpression
    );

    node->location.begin = base->location.begin;

    appendChild(
        *node,
        std::move(base)
    );

    expect(
        TokenKind::LeftBracket,
        "expected opening bracket for subscript"
    );

    while (!atEnd() && !check(TokenKind::RightBracket)) {
        appendChild(
            *node,
            parseExpression()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightBracket,
        "expected closing bracket for subscript"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseClosureExpression() {
    auto node = makeNode(
        SyntaxKind::ClosureExpression
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::RightBrace)) {
        appendChild(
            *node,
            parseStatement()
        );
    }

    expect(
        TokenKind::RightBrace,
        "expected closing brace for closure"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseTupleExpression() {
    auto node = makeNode(
        SyntaxKind::TupleExpression
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::RightParen)) {
        appendChild(
            *node,
            parseExpression()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightParen,
        "expected closing parenthesis"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseArrayExpression() {
    auto node = makeNode(
        SyntaxKind::ArrayExpression
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::RightBracket)) {
        appendChild(
            *node,
            parseExpression()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightBracket,
        "expected closing bracket"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseDictionaryExpression() {
    auto node = makeNode(
        SyntaxKind::DictionaryExpression
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::RightBracket)) {
        appendChild(
            *node,
            parseExpression()
        );

        if (consume(TokenKind::Colon)) {
            appendChild(
                *node,
                parseExpression()
            );
        }

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightBracket,
        "expected closing bracket for dictionary"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseType() {
    if (check(TokenKind::Identifier) || isBuiltinTypeToken(current().kind)) {
        auto type = parseNamedType();

        if (check(TokenKind::Less)) {
            return parseGenericType();
        }

        if (consume(TokenKind::Question)) {
            auto optional = makeNode(
                SyntaxKind::OptionalType
            );

            appendChild(
                *optional,
                std::move(type)
            );

            return optional;
        }

        return type;
    }

    if (check(TokenKind::LeftParen)) {
        return parseTupleType();
    }

    diagnose(
        current().location,
        ParseDiagnostic::Severity::Error,
        "expected type"
    );

    return makeNode(
        SyntaxKind::Unknown
    );
}

std::unique_ptr<SyntaxNode> Parser::parseNamedType() {
    auto node = makeNode(
        SyntaxKind::NamedType
    );

    node->location.begin = current().location;
    node->text = tokenText(
        advance()
    );

    while (consume(TokenKind::Dot)) {
        if (!check(TokenKind::Identifier)) {
            diagnose(
                current().location,
                ParseDiagnostic::Severity::Error,
                "expected type member name"
            );
            break;
        }

        auto member = makeNode(
            SyntaxKind::NamedType
        );

        member->text = tokenText(
            advance()
        );

        appendChild(
            *node,
            std::move(member)
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseGenericType() {
    auto node = makeNode(
        SyntaxKind::GenericType
    );

    node->location.begin = current().location;

    if (check(TokenKind::Identifier) || isBuiltinTypeToken(current().kind)) {
        node->text = tokenText(
            advance()
        );
    }

    expect(
        TokenKind::Less,
        "expected opening generic argument list"
    );

    while (!atEnd() && !check(TokenKind::Greater)) {
        appendChild(
            *node,
            parseType()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::Greater,
        "expected closing generic argument list"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseFunctionType() {
    auto node = makeNode(
        SyntaxKind::FunctionType
    );

    node->location.begin = current().location;

    appendChild(
        *node,
        parseTupleType()
    );

    expect(
        TokenKind::Arrow,
        "expected arrow in function type"
    );

    appendChild(
        *node,
        parseType()
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseTupleType() {
    auto node = makeNode(
        SyntaxKind::TupleType
    );

    node->location.begin = current().location;

    expect(
        TokenKind::LeftParen,
        "expected opening parenthesis for tuple type"
    );

    while (!atEnd() && !check(TokenKind::RightParen)) {
        appendChild(
            *node,
            parseType()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightParen,
        "expected closing parenthesis for tuple type"
    );

    if (consume(TokenKind::Arrow)) {
        auto function = makeNode(
            SyntaxKind::FunctionType
        );

        appendChild(
            *function,
            std::move(node)
        );

        appendChild(
            *function,
            parseType()
        );

        function->location.end = previous().location;
        return function;
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseTypeAnnotation() {
    auto node = makeNode(
        SyntaxKind::TypeAnnotation
    );

    node->location.begin = previous().location;
    appendChild(
        *node,
        parseType()
    );
    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseParameterClause() {
    auto node = makeNode(
        SyntaxKind::ParameterClause
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::RightParen)) {
        appendChild(
            *node,
            parseParameter()
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::RightParen,
        "expected closing parameter parenthesis"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseParameter() {
    auto node = makeNode(
        SyntaxKind::Parameter
    );

    node->location.begin = current().location;

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected parameter name"
        );
    }

    if (consume(TokenKind::Colon)) {
        appendChild(
            *node,
            parseType()
        );
    }

    if (consume(TokenKind::Equal)) {
        appendChild(
            *node,
            parseExpression()
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseGenericParameterClause() {
    auto node = makeNode(
        SyntaxKind::GenericParameterClause
    );

    node->location.begin = current().location;
    advance();

    while (!atEnd() && !check(TokenKind::Greater)) {
        auto parameter = makeNode(
            SyntaxKind::GenericParameter
        );

        if (check(TokenKind::Identifier)) {
            parameter->text = tokenText(
                advance()
            );
        } else {
            diagnose(
                current().location,
                ParseDiagnostic::Severity::Error,
                "expected generic parameter name"
            );
            break;
        }

        if (consume(TokenKind::Colon)) {
            appendChild(
                *parameter,
                parseType()
            );
        }

        appendChild(
            *node,
            std::move(parameter)
        );

        if (!consume(TokenKind::Comma)) {
            break;
        }
    }

    expect(
        TokenKind::Greater,
        "expected closing generic parameter list"
    );

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseAttribute() {
    auto node = makeNode(
        SyntaxKind::Attribute
    );

    node->location.begin = current().location;
    advance();

    if (check(TokenKind::Identifier)) {
        node->text = tokenText(
            advance()
        );
    } else {
        diagnose(
            current().location,
            ParseDiagnostic::Severity::Error,
            "expected attribute name"
        );
    }

    if (check(TokenKind::LeftParen)) {
        appendChild(
            *node,
            parseCallExpression(
                makeNode(SyntaxKind::IdentifierExpression)
            )
        );
    }

    node->location.end = previous().location;
    return node;
}

std::unique_ptr<SyntaxNode> Parser::parseModifier() {
    auto node = makeNode(
        SyntaxKind::Modifier
    );

    node->location.begin = current().location;
    node->text = tokenText(
        advance()
    );
    node->location.end = previous().location;
    return node;
}

bool Parser::isDeclarationStart(
    TokenKind kind
) noexcept {
    switch (kind) {
        case TokenKind::KwImport:
        case TokenKind::KwFunc:
        case TokenKind::KwVar:
        case TokenKind::KwLet:
        case TokenKind::KwConst:
        case TokenKind::KwType:
        case TokenKind::KwStruct:
        case TokenKind::KwClass:
        case TokenKind::KwEnum:
        case TokenKind::KwProtocol:
        case TokenKind::KwExtension:
            return true;
        default:
            return false;
    }
}

bool Parser::isStatementStart(
    TokenKind kind
) noexcept {
    if (isDeclarationStart(kind)) {
        return true;
    }

    switch (kind) {
        case TokenKind::LeftBrace:
        case TokenKind::KwIf:
        case TokenKind::KwWhile:
        case TokenKind::KwFor:
        case TokenKind::KwDo:
        case TokenKind::KwGuard:
        case TokenKind::KwSwitch:
        case TokenKind::KwReturn:
        case TokenKind::KwThrow:
        case TokenKind::KwDefer:
        case TokenKind::Identifier:
        case TokenKind::IntegerLiteral:
        case TokenKind::FloatLiteral:
        case TokenKind::StringLiteral:
        case TokenKind::CharacterLiteral:
        case TokenKind::LeftParen:
        case TokenKind::LeftBracket:
            return true;
        default:
            return false;
    }
}

bool Parser::isLiteral(
    TokenKind kind
) noexcept {
    switch (kind) {
        case TokenKind::IntegerLiteral:
        case TokenKind::BinaryIntegerLiteral:
        case TokenKind::OctalIntegerLiteral:
        case TokenKind::HexIntegerLiteral:
        case TokenKind::FloatLiteral:
        case TokenKind::HexFloatLiteral:
        case TokenKind::StringLiteral:
        case TokenKind::MultilineStringLiteral:
        case TokenKind::CharacterLiteral:
        case TokenKind::RegexLiteral:
        case TokenKind::KwTrue:
        case TokenKind::KwFalse:
        case TokenKind::KwNil:
            return true;
        default:
            return false;
    }
}

bool Parser::isTypeStart(
    TokenKind kind
) noexcept {
    return kind == TokenKind::Identifier
        || isBuiltinTypeToken(kind)
        || kind == TokenKind::LeftParen;
}

bool Parser::isAssignmentOperator(
    TokenKind kind
) noexcept {
    switch (kind) {
        case TokenKind::Equal:
        case TokenKind::PlusEqual:
        case TokenKind::MinusEqual:
        case TokenKind::StarEqual:
        case TokenKind::SlashEqual:
        case TokenKind::PercentEqual:
        case TokenKind::AmpersandEqual:
        case TokenKind::PipeEqual:
        case TokenKind::CaretEqual:
        case TokenKind::ShiftLeftEqual:
        case TokenKind::ShiftRightEqual:
            return true;
        default:
            return false;
    }
}

int Parser::precedence(
    TokenKind kind
) noexcept {
    switch (kind) {
        case TokenKind::OrOr:
        case TokenKind::Or:
            return 10;
        case TokenKind::AndAnd:
        case TokenKind::Ampersand:
            return 20;
        case TokenKind::EqualEqual:
        case TokenKind::NotEqual:
        case TokenKind::TildeEqual:
            return 30;
        case TokenKind::Greater:
        case TokenKind::GreaterEqual:
        case TokenKind::Less:
        case TokenKind::LessEqual:
            return 40;
        case TokenKind::ShiftLeft:
        case TokenKind::ShiftRight:
            return 50;
        case TokenKind::Plus:
        case TokenKind::Minus:
        case TokenKind::Pipe:
        case TokenKind::Caret:
            return 60;
        case TokenKind::Star:
        case TokenKind::Slash:
        case TokenKind::Percent:
            return 70;
        case TokenKind::Power:
            return 80;
        case TokenKind::Range:
        case TokenKind::ClosedRange:
        case TokenKind::QuestionQuestion:
            return 15;
        default:
            return -1;
    }
}

SyntaxKind Parser::syntaxForToken(
    TokenKind kind
) noexcept {
    switch (kind) {
        case TokenKind::Identifier:
            return SyntaxKind::IdentifierExpression;
        case TokenKind::IntegerLiteral:
        case TokenKind::BinaryIntegerLiteral:
        case TokenKind::OctalIntegerLiteral:
        case TokenKind::HexIntegerLiteral:
        case TokenKind::FloatLiteral:
        case TokenKind::HexFloatLiteral:
        case TokenKind::StringLiteral:
        case TokenKind::MultilineStringLiteral:
        case TokenKind::CharacterLiteral:
        case TokenKind::RegexLiteral:
        case TokenKind::KwTrue:
        case TokenKind::KwFalse:
        case TokenKind::KwNil:
            return SyntaxKind::LiteralExpression;
        case TokenKind::KwFunc:
            return SyntaxKind::FunctionDeclaration;
        case TokenKind::KwVar:
        case TokenKind::KwLet:
        case TokenKind::KwConst:
            return SyntaxKind::VariableDeclaration;
        case TokenKind::KwStruct:
            return SyntaxKind::StructDeclaration;
        case TokenKind::KwClass:
            return SyntaxKind::ClassDeclaration;
        case TokenKind::KwEnum:
            return SyntaxKind::EnumDeclaration;
        case TokenKind::KwProtocol:
            return SyntaxKind::ProtocolDeclaration;
        case TokenKind::KwExtension:
            return SyntaxKind::ExtensionDeclaration;
        case TokenKind::KwIf:
            return SyntaxKind::IfStatement;
        case TokenKind::KwWhile:
            return SyntaxKind::WhileStatement;
        case TokenKind::KwFor:
            return SyntaxKind::ForStatement;
        case TokenKind::KwReturn:
            return SyntaxKind::ReturnStatement;
        case TokenKind::KwThrow:
            return SyntaxKind::ThrowStatement;
        case TokenKind::KwDefer:
            return SyntaxKind::DeferStatement;
        case TokenKind::KwSwitch:
            return SyntaxKind::SwitchStatement;
        case TokenKind::KwCase:
            return SyntaxKind::CaseStatement;
        case TokenKind::LeftBrace:
            return SyntaxKind::CompoundStatement;
        case TokenKind::LeftParen:
            return SyntaxKind::TupleExpression;
        case TokenKind::LeftBracket:
            return SyntaxKind::ArrayExpression;
        default:
            return SyntaxKind::Unknown;
    }
}

std::string Parser::tokenText(
    const Token& token
) {
    return token.text;
}

const char* syntaxKindName(
    SyntaxKind kind
) noexcept {
    switch (kind) {
        case SyntaxKind::TranslationUnit: return "TranslationUnit";
        case SyntaxKind::Declaration: return "Declaration";
        case SyntaxKind::ImportDeclaration: return "ImportDeclaration";
        case SyntaxKind::FunctionDeclaration: return "FunctionDeclaration";
        case SyntaxKind::VariableDeclaration: return "VariableDeclaration";
        case SyntaxKind::TypeDeclaration: return "TypeDeclaration";
        case SyntaxKind::StructDeclaration: return "StructDeclaration";
        case SyntaxKind::ClassDeclaration: return "ClassDeclaration";
        case SyntaxKind::EnumDeclaration: return "EnumDeclaration";
        case SyntaxKind::ProtocolDeclaration: return "ProtocolDeclaration";
        case SyntaxKind::ExtensionDeclaration: return "ExtensionDeclaration";
        case SyntaxKind::ParameterClause: return "ParameterClause";
        case SyntaxKind::Parameter: return "Parameter";
        case SyntaxKind::GenericParameterClause: return "GenericParameterClause";
        case SyntaxKind::GenericParameter: return "GenericParameter";
        case SyntaxKind::ReturnClause: return "ReturnClause";
        case SyntaxKind::Attribute: return "Attribute";
        case SyntaxKind::Modifier: return "Modifier";
        case SyntaxKind::Statement: return "Statement";
        case SyntaxKind::CompoundStatement: return "CompoundStatement";
        case SyntaxKind::ExpressionStatement: return "ExpressionStatement";
        case SyntaxKind::IfStatement: return "IfStatement";
        case SyntaxKind::WhileStatement: return "WhileStatement";
        case SyntaxKind::ForStatement: return "ForStatement";
        case SyntaxKind::DoStatement: return "DoStatement";
        case SyntaxKind::GuardStatement: return "GuardStatement";
        case SyntaxKind::SwitchStatement: return "SwitchStatement";
        case SyntaxKind::CaseStatement: return "CaseStatement";
        case SyntaxKind::BreakStatement: return "BreakStatement";
        case SyntaxKind::ContinueStatement: return "ContinueStatement";
        case SyntaxKind::ReturnStatement: return "ReturnStatement";
        case SyntaxKind::ThrowStatement: return "ThrowStatement";
        case SyntaxKind::DeferStatement: return "DeferStatement";
        case SyntaxKind::DeclarationStatement: return "DeclarationStatement";
        case SyntaxKind::Expression: return "Expression";
        case SyntaxKind::IdentifierExpression: return "IdentifierExpression";
        case SyntaxKind::LiteralExpression: return "LiteralExpression";
        case SyntaxKind::PrefixExpression: return "PrefixExpression";
        case SyntaxKind::PostfixExpression: return "PostfixExpression";
        case SyntaxKind::BinaryExpression: return "BinaryExpression";
        case SyntaxKind::CallExpression: return "CallExpression";
        case SyntaxKind::MemberExpression: return "MemberExpression";
        case SyntaxKind::SubscriptExpression: return "SubscriptExpression";
        case SyntaxKind::AssignmentExpression: return "AssignmentExpression";
        case SyntaxKind::TupleExpression: return "TupleExpression";
        case SyntaxKind::ArrayExpression: return "ArrayExpression";
        case SyntaxKind::DictionaryExpression: return "DictionaryExpression";
        case SyntaxKind::ClosureExpression: return "ClosureExpression";
        case SyntaxKind::StringExpression: return "StringExpression";
        case SyntaxKind::TypeAnnotation: return "TypeAnnotation";
        case SyntaxKind::NamedType: return "NamedType";
        case SyntaxKind::OptionalType: return "OptionalType";
        case SyntaxKind::FunctionType: return "FunctionType";
        case SyntaxKind::TupleType: return "TupleType";
        case SyntaxKind::ArrayType: return "ArrayType";
        case SyntaxKind::GenericType: return "GenericType";
        case SyntaxKind::Unknown: return "Unknown";
    }

    return "Unknown";
}

// Parser implementation notes are kept close to the routines they describe.
// The parser consumes the lexer stream without changing lexer ownership.
// Recovery always attempts to make forward progress.
// Declarations own their nested syntax nodes.
// Expressions use precedence climbing for binary operators.
// Types are parsed independently so declarations can reuse them.
// Compound statements own their contained statement sequence.
// Diagnostics retain the source location supplied by the lexer.
// Optional parser features are controlled through ParserOptions.
// The parser accepts incomplete input when recovery is enabled.
// Top-level statements remain available for HyperLang scripts.

} // namespace hyper
