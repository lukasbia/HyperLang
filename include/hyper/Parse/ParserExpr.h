#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

enum class ExpressionKind {
    Invalid,
    Identifier,
    IntegerLiteral,
    FloatingLiteral,
    StringLiteral,
    BooleanLiteral,
    NilLiteral,
    Unary,
    Binary,
    Assignment,
    Call,
    Member,
    Subscript,
    Tuple,
    Array,
    Dictionary,
    Closure,
    Conditional,
    Cast,
    Await,
    Move
};

struct ExpressionSyntax {
    ExpressionKind kind = ExpressionKind::Invalid;
    std::string text;
    std::string operatorText;
    std::vector<ExpressionSyntax> children;
};

} // namespace hyper::parse
