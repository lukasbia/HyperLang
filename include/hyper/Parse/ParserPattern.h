#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

enum class PatternKind {
    Invalid, Wildcard, Identifier, Literal, Tuple, EnumCase,
    Optional, TypeCast, ValueBinding, Expression, IsType, AsType
};

struct PatternSyntax {
    PatternKind kind = PatternKind::Invalid;
    std::string text;
    std::vector<PatternSyntax> children;
};

class PatternParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseWildcard(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseIdentifier(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseTuple(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseEnumCase(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseTypeCast(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseValueBinding(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseExpressionPattern(Parser& parser);
};

} // namespace hyper::parse
