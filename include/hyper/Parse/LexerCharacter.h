#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace hyper {

class LexerCharacter {
public:
    static bool isASCIIIdentifierStart(char) noexcept;
    static bool isASCIIIdentifierContinue(char) noexcept;
    static bool isASCIIDigit(char) noexcept;
    static bool isASCIIHexDigit(char) noexcept;
    static bool isASCIIOctalDigit(char) noexcept;
    static bool isASCIIWhitespace(char) noexcept;
    static bool isPrintableASCII(char) noexcept;
    static bool isUnicodeIdentifierStart(std::uint32_t) noexcept;
    static bool isUnicodeIdentifierContinue(std::uint32_t) noexcept;
    static bool isForbiddenIdentifierCodePoint(std::uint32_t) noexcept;
    static bool isOperatorCharacter(char) noexcept;
    static bool isOperatorStartCharacter(char) noexcept;
    static bool isOperatorContinuationCharacter(char) noexcept;
    static std::uint32_t decodeUTF8(std::string_view source, std::size_t& offset) noexcept;
    static bool encodeUTF8(std::uint32_t codePoint, std::string& output);
    static bool validateUTF8(std::string_view source) noexcept;
};

} // namespace hyper
