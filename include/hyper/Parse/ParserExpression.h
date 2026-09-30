#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace hyper {

enum class ExpressionKind {
    Invalid,
    Identifier,
    Literal,
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
    StringInterpolation,
    Conditional,
    KeyPath,
    MacroExpansion,
    OptionalChaining,
    ForceUnwrap,
    Cast,
    Await,
    Try
};

struct ExpressionNode {
    ExpressionKind kind = ExpressionKind::Invalid;
    std::string text;
    std::string typeSpelling;
    std::string operatorSpelling;
    std::vector<ExpressionNode> children;
    bool parenthesized = false;
    bool trailingClosure = false;
    bool optionalChaining = false;
    bool forceUnwrapped = false;
    bool isAwaited = false;
    bool isTryExpression = false;
};

class ExpressionParserSupport {
public:
    static bool isLiteral(
        ExpressionKind kind
    ) noexcept;

    static bool isCallable(
        ExpressionKind kind
    ) noexcept;

    static bool isPostfix(
        ExpressionKind kind
    ) noexcept;

    static bool isPrimary(
        ExpressionKind kind
    ) noexcept;

    static bool isAssignment(
        ExpressionKind kind
    ) noexcept;

    static int precedenceForOperator(
        std::string_view spelling
    ) noexcept;
};

} // namespace hyper
