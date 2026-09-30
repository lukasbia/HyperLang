#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

enum class ExpressionKind {
    Invalid, Identifier, IntegerLiteral, FloatingLiteral, StringLiteral,
    BooleanLiteral, NilLiteral, Unary, Binary, Assignment, Call, Member,
    Subscript, Tuple, Array, Dictionary, Closure, Conditional, Cast,
    Await, Move
};

struct ExpressionSyntax {
    ExpressionKind kind = ExpressionKind::Invalid;
    std::string text;
    std::string operatorText;
    std::vector<ExpressionSyntax> children;
};

class ExpressionParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseAssignment(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseBinary(Parser& parser, int minimumPrecedence);
    static std::unique_ptr<SyntaxNode> parsePrefix(Parser& parser);
    static std::unique_ptr<SyntaxNode> parsePostfix(Parser& parser);
    static std::unique_ptr<SyntaxNode> parsePrimary(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseIdentifier(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseLiteral(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseCall(Parser& parser, std::unique_ptr<SyntaxNode> base);
    static std::unique_ptr<SyntaxNode> parseMember(Parser& parser, std::unique_ptr<SyntaxNode> base);
    static std::unique_ptr<SyntaxNode> parseSubscript(Parser& parser, std::unique_ptr<SyntaxNode> base);
    static std::unique_ptr<SyntaxNode> parseClosure(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseTuple(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseArray(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseDictionary(Parser& parser);
    static int precedence(TokenKind kind) noexcept;
};

} // namespace hyper::parse
