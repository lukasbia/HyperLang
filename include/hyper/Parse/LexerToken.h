#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace hyper {

enum class TokenKind : std::uint16_t;
enum class OperatorBinding : std::uint8_t;
enum class NumberBase : std::uint8_t;
enum class StringKind : std::uint8_t;

struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation location;
    OperatorBinding binding;
    NumberBase numberBase;
    StringKind stringKind;
    bool atStartOfLine = false;
    bool escapedIdentifier = false;
    bool malformed = false;
    bool hasLeadingComment = false;

    std::size_t endOffset() const noexcept {
        return location.offset + text.size();
    }
};

std::string_view tokenText(const Token&) noexcept;
bool tokenIsTrivia(TokenKind) noexcept;
bool tokenIsLiteral(TokenKind) noexcept;
bool tokenIsKeyword(TokenKind) noexcept;
bool tokenIsOperator(TokenKind) noexcept;

} // namespace hyper
