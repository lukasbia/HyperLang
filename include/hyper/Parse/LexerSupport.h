#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace hyper {

struct SourceLocation;
enum class NumberBase : std::uint8_t;
enum class StringKind : std::uint8_t;
enum class TokenKind;
enum class OperatorBinding;

struct LexerRange {
    std::size_t begin = 0;
    std::size_t end = 0;
    constexpr std::size_t size() const noexcept {
        return end >= begin ? end - begin : 0;
    }
};

struct NumericLiteralInfo {
    NumberBase base;
    bool hasDecimalPoint = false;
    bool hasExponent = false;
    bool exponentIsBinary = false;
    bool hasSeparators = false;
    bool hasLeadingZero = false;
    bool hasSuffix = false;
};

struct StringLiteralInfo {
    StringKind kind;
    unsigned customDelimiterLength = 0;
    bool hasInterpolation = false;
    bool hasEscapes = false;
    bool terminated = true;
};

struct LexerDiagnostic {
    enum class Severity {
        Note,
        Warning,
        Error
    };
    Severity severity = Severity::Error;
    SourceLocation location;
    std::string message;
    std::string replacement;
};

const char* tokenKindName(TokenKind) noexcept;
const char* operatorBindingName(OperatorBinding) noexcept;

} // namespace hyper
