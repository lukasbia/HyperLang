//===--- LexerUnicode.h - HyperLang Lexer Unicode Facilities -------------===//

#ifndef HYPERLANG_PARSE_LEXER_UNICODE_H
#define HYPERLANG_PARSE_LEXER_UNICODE_H

#include "hyperlang/Basic/UTF8.h"

#include <string_view>

namespace hyperlang::lexer {

inline bool isUnicodeIdentifierStart(std::string_view source,
                                     std::size_t offset) noexcept {
  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierStart(result.codePoint);
}

inline bool isUnicodeIdentifierContinue(std::string_view source,
                                        std::size_t offset) noexcept {
  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierContinue(result.codePoint);
}

inline bool isUnicodeWhitespace(std::string_view source,
                                std::size_t offset) noexcept {
  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isWhitespace(result.codePoint);
}

inline bool isSingleUnicodeScalar(std::string_view source) noexcept {
  if (source.empty())
    return false;

  std::size_t offset = 0;
  unsigned count = 0;
  while (offset < source.size()) {
    const unicode::DecodeResult result = unicode::decode(source, offset);
    if (!result.valid)
      return false;
    offset += result.width;
    ++count;
  }
  return count == 1;
}

} // namespace hyperlang::lexer

#endif
