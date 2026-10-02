//===--- UTF8.h - HyperLang UTF-8 Utilities -------------------------------===//

#ifndef HYPERLANG_BASIC_UTF8_H
#define HYPERLANG_BASIC_UTF8_H

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace hyperlang::unicode {

struct DecodeResult {
  std::uint32_t codePoint;
  std::size_t width;
  bool valid;
};

DecodeResult decode(std::string_view text, std::size_t offset);
bool isContinuationByte(unsigned char byte);
bool isValidScalar(std::uint32_t codePoint);
bool isIdentifierStart(std::uint32_t codePoint);
bool isIdentifierContinue(std::uint32_t codePoint);
bool isWhitespace(std::uint32_t codePoint);
std::size_t encodedLength(std::uint32_t codePoint);
bool validate(std::string_view text);

} // namespace hyperlang::unicode

#endif
