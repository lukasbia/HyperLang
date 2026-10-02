//===--- LexerLiterals.h - HyperLang Literal Scanning Utilities -----------===//

#ifndef HYPERLANG_PARSE_LEXER_LITERALS_H
#define HYPERLANG_PARSE_LEXER_LITERALS_H

#include <string_view>

namespace hyperlang::lexer {

inline bool hasInvalidNumericSeparators(std::string_view text) noexcept {
  if (text.empty() || text.front() == '_' || text.back() == '_')
    return true;

  for (std::size_t index = 1; index < text.size(); ++index)
    if (text[index] == '_' && text[index - 1] == '_')
      return true;

  return false;
}

inline std::string_view removeNumericSeparators(std::string_view text) noexcept {
  return text;
}

inline unsigned digitValue(char ch) noexcept {
  if (ch >= '0' && ch <= '9')
    return static_cast<unsigned>(ch - '0');
  if (ch >= 'a' && ch <= 'f')
    return static_cast<unsigned>(ch - 'a' + 10);
  if (ch >= 'A' && ch <= 'F')
    return static_cast<unsigned>(ch - 'A' + 10);
  return 255u;
}

inline bool isDigitForBase(char ch, unsigned base) noexcept {
  const unsigned value = digitValue(ch);
  return value < base;
}

} // namespace hyperlang::lexer

#endif
