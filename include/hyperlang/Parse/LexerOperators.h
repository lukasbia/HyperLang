//===--- LexerOperators.h - HyperLang Operator Classification -------------===//

#ifndef HYPERLANG_PARSE_LEXER_OPERATORS_H
#define HYPERLANG_PARSE_LEXER_OPERATORS_H

namespace hyperlang::lexer {

constexpr bool isOperatorCharacter(char ch) noexcept {
  switch (ch) {
  case '+': case '-': case '*': case '/': case '%':
  case '=': case '!': case '<': case '>': case '&':
  case '|': case '^': case '~': case '?': case '.':
  case ':': case '\\':
    return true;
  default:
    return false;
  }
}

constexpr bool isPunctuationCharacter(char ch) noexcept {
  switch (ch) {
  case '+': case '-': case '*': case '/': case '%':
  case '=': case '!': case '<': case '>': case '&':
  case '|': case '^': case '~': case '?': case '.':
  case ':': case ',': case ';': case '(': case ')':
  case '{': case '}': case '[': case ']': case '\\': case '#':
    return true;
  default:
    return false;
  }
}

} // namespace hyperlang::lexer

#endif
