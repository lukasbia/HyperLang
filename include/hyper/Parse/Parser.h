#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "hyper/Parse/Lexer.h"
#include "hyper/Parse/ParserCore.h"
#include "hyper/Parse/ParserDeclaration.h"
#include "hyper/Parse/ParserDiagnostics.h"
#include "hyper/Parse/ParserExpression.h"
#include "hyper/Parse/ParserDecl.h"
#include "hyper/Parse/ParserDeclName.h"
#include "hyper/Parse/ParserExpr.h"
#include "hyper/Parse/ParserGeneric.h"
#include "hyper/Parse/ParserIfConfig.h"
#include "hyper/Parse/ParserPattern.h"
#include "hyper/Parse/ParserRegex.h"
#include "hyper/Parse/ParserRequests.h"
#include "hyper/Parse/ParserStmt.h"
#include "hyper/Parse/ParserType.h"
#include "hyper/Parse/ParserVersion.h"
#include "hyper/Parse/PersistentParserState.h"

namespace hyper {

class Parser {
public:
    explicit Parser(std::string_view source);
    explicit Parser(std::vector<Token> tokens);
    ~Parser();

    Parser(Parser&&) noexcept;
    Parser& operator=(Parser&&) noexcept;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::unique_ptr<SyntaxNode> parse();
    std::unique_ptr<SyntaxNode> parseTranslationUnit();

    const std::vector<ParseDiagnostic>& diagnostics() const noexcept;
    bool hasErrors() const noexcept;
    void clearDiagnostics();

    void reset();
    bool atEnd() const noexcept;

    std::unique_ptr<SyntaxNode> parseDeclaration();
    std::unique_ptr<SyntaxNode> parseStatement();
    std::unique_ptr<SyntaxNode> parseExpression();
    std::unique_ptr<SyntaxNode> parseType();
    std::unique_ptr<SyntaxNode> parsePattern();
    std::unique_ptr<SyntaxNode> parseGenericClause();

    std::unique_ptr<SyntaxNode> parseImportDeclaration();
    std::unique_ptr<SyntaxNode> parseFunctionDeclaration();
    std::unique_ptr<SyntaxNode> parseVariableDeclaration();
    std::unique_ptr<SyntaxNode> parseTypeDeclaration();
    std::unique_ptr<SyntaxNode> parseCompoundStatement();
    std::unique_ptr<SyntaxNode> parseIfStatement();
    std::unique_ptr<SyntaxNode> parseWhileStatement();
    std::unique_ptr<SyntaxNode> parseForStatement();
    std::unique_ptr<SyntaxNode> parseSwitchStatement();
    std::unique_ptr<SyntaxNode> parseReturnStatement();

private:
    struct Implementation;
    std::unique_ptr<Implementation> implementation_;
};

} // namespace hyper
