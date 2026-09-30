#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace hyper {

enum class TokenKind : std::uint16_t;
enum class OperatorBinding : std::uint8_t;
enum class NumberBase : std::uint8_t;
enum class StringKind : std::uint8_t;
struct SourceLocation;

struct NumericLiteralInfo {
    NumberBase base{};
    bool hasDecimalPoint = false;
    bool hasExponent = false;
    bool exponentIsBinary = false;
    bool hasSeparators = false;
    bool hasLeadingZero = false;
    bool hasSuffix = false;
};

struct StringLiteralInfo {
    StringKind kind{};
    unsigned customDelimiterLength = 0;
    bool hasInterpolation = false;
    bool hasEscapes = false;
    bool terminated = true;
};

struct Token {
    TokenKind kind{};
    std::string text;
    SourceLocation location{};
    OperatorBinding binding{};
    NumericLiteralInfo numeric{};
    StringLiteralInfo string{};
    bool atStartOfLine = false;
    bool escapedIdentifier = false;
    bool malformed = false;
    bool hasLeadingComment = false;

    std::size_t endOffset() const noexcept;
    bool isIdentifier() const noexcept;
    bool isLiteral() const noexcept;
    bool isOperator() const noexcept;
    bool isTrivia() const noexcept;
};

} // namespace hyper
