//===--- LexerTrivia.h - HyperLang Trivia Classification -----------------===//

#ifndef HYPERLANG_PARSE_LEXER_TRIVIA_H
#define HYPERLANG_PARSE_LEXER_TRIVIA_H

#include <string_view>

namespace hyperlang::lexer {

constexpr bool isASCIIWhitespace(char ch) noexcept {
  switch (ch) {
  case ' ': case '\t': case '\n': case '\r': case '\v': case '\f':
    return true;
  default:
    return false;
  }
}

constexpr bool isLineTerminator(char ch) noexcept {
  return ch == '\n' || ch == '\r';
}

constexpr bool isHashbang(std::string_view source) noexcept {
  return source.size() >= 2 && source[0] == '#' && source[1] == '!';
}

constexpr bool isCommentStart(std::string_view source,
                              std::size_t offset) noexcept {
  return offset + 1 < source.size() && source[offset] == '/' &&
         (source[offset + 1] == '/' || source[offset + 1] == '*');
}

} // namespace hyperlang::lexer

#endif
