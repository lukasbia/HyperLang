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
    bool isInteger = false;
    bool isFloating = false;
    bool isHexadecimal = false;
    bool isBinary = false;
    bool isOctal = false;
};

struct StringLiteralInfo {
    StringKind kind{};
    unsigned customDelimiterLength = 0;
    bool hasInterpolation = false;
    bool hasEscapes = false;
    bool terminated = true;
    bool multiline = false;
    bool raw = false;
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
    bool hasTrailingComment = false;
    bool isDocumentation = false;
    bool isDirective = false;

    std::size_t endOffset() const noexcept;
    bool isIdentifier() const noexcept;
    bool isLiteral() const noexcept;
    bool isOperator() const noexcept;
    bool isTrivia() const noexcept;
    bool isKeyword() const noexcept;
    bool isPunctuation() const noexcept;
};

} // namespace hyper
