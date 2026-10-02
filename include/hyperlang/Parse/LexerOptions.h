//===--- LexerOptions.h - HyperLang Lexer Configuration ------------------===//

#ifndef HYPERLANG_PARSE_LEXER_OPTIONS_H
#define HYPERLANG_PARSE_LEXER_OPTIONS_H

namespace hyperlang {

struct LexerOptions {
  bool retainComments = false;
  bool allowHashbang = true;
  bool diagnoseUnknownCharacters = true;
  bool allowEscapedIdentifiers = true;
  bool allowUnicodeIdentifiers = true;
  bool allowNestedBlockComments = true;
};

} // namespace hyperlang

#endif
