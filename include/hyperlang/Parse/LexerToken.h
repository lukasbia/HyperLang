//===--- LexerToken.h - HyperLang Lexer Token Model ---------------------===//

#ifndef HYPERLANG_PARSE_LEXER_TOKEN_H
#define HYPERLANG_PARSE_LEXER_TOKEN_H

#include "hyperlang/Parse/TokenKinds.h"
#include "hyperlang/Parse/LexerDiagnostics.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace hyperlang {

struct Token {
  tok::Kind kind = tok::Kind::Unknown;
  SourceRange range{};
  std::string_view text{};
  std::string decodedText{};
  std::uint64_t integerValue = 0;
  double floatingValue = 0.0;
  bool hasIntegerValue = false;
  bool hasFloatingValue = false;

  bool is(tok::Kind expected) const { return kind == expected; }

  bool isIdentifier() const {
    return kind == tok::Kind::Identifier ||
           kind == tok::Kind::EscapedIdentifier;
  }

  bool isLiteral() const {
    return kind == tok::Kind::IntegerLiteral ||
           kind == tok::Kind::FloatingLiteral ||
           kind == tok::Kind::StringLiteral ||
           kind == tok::Kind::CharacterLiteral;
  }

  bool isTrivia() const {
    return kind == tok::Kind::Comment;
  }
};

} // namespace hyperlang

#endif
