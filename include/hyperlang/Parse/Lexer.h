//===--- Lexer.h - HyperLang Lexer ---------------------------------------===//

#ifndef HYPERLANG_PARSE_LEXER_H
#define HYPERLANG_PARSE_LEXER_H

#include "hyperlang/Parse/TokenKinds.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace hyperlang {

struct SourceLocation {
  std::size_t offset = 0;
  std::size_t line = 1;
  std::size_t column = 1;
};

struct SourceRange {
  SourceLocation start;
  SourceLocation end;
};

struct Diagnostic {
  enum class Severity { Note, Warning, Error };
  Severity severity;
  SourceLocation location;
  std::string message;
};

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
};

struct LexerOptions {
  bool retainComments = false;
  bool allowHashbang = true;
  bool diagnoseUnknownCharacters = true;
  bool allowEscapedIdentifiers = true;
  bool allowUnicodeIdentifiers = true;
  bool allowNestedBlockComments = true;
};

class Lexer {
public:
  explicit Lexer(std::string_view source, LexerOptions options = {});
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;

  Token lex();
  const Token &peek();
  Token lexIf(tok::Kind kind);

  bool atEnd() const;
  std::size_t offset() const;
  SourceLocation location() const;
  const std::vector<Diagnostic> &diagnostics() const;
  bool hasErrors() const;

  void reset(std::size_t offset = 0);
  void setRetainComments(bool enabled);

  static tok::Kind classifyIdentifier(std::string_view identifier);
  static bool isKeyword(std::string_view identifier);
  static bool isAttribute(std::string_view spelling);
  static bool isOperatorCharacter(char ch);

private:
  std::string_view source_;
  LexerOptions options_;
  std::size_t cursor_ = 0;
  std::size_t line_ = 1;
  std::size_t column_ = 1;
  std::optional<Token> lookahead_;
  std::vector<Diagnostic> diagnostics_;

  SourceLocation currentLocation() const;
  char currentByte() const;
  char peekByte(std::size_t distance = 1) const;
  void advanceByte();
  void advanceBytes(std::size_t count);
  bool consumeIf(char ch);

  void emit(Diagnostic::Severity severity, SourceLocation location, std::string message);
  void error(SourceLocation location, std::string message);
  void warning(SourceLocation location, std::string message);

  Token makeToken(tok::Kind kind, std::size_t start, SourceLocation startLocation) const;
  Token lexImpl();

  bool skipTrivia();
  bool skipHorizontalWhitespace();
  bool skipLineComment();
  bool scanUTF8CodePoint();
  bool consumeUTF8BOM();
  bool isAtLineStart() const;
  bool skipBlockComment();
  bool skipHashbang();

  Token lexIdentifierOrKeyword();
  Token lexEscapedIdentifier();
  Token lexAttributedName();
  Token lexComment();
  Token makeEOFToken() const;
  Token lexNumber();
  Token lexString();
  Token lexCharacter();
  Token lexOperatorOrPunctuation();
  Token lexUnknown();

  bool scanIdentifier();
  bool scanIdentifierContinuation();
  bool scanDigits(unsigned base, bool requireDigit);
  bool scanExponent();
  bool scanFloatSuffix();

  bool decodeEscape(std::string &output);
  bool decodeUnicodeEscape(std::string &output);
  bool decodeHexEscape(std::string &output, unsigned digits);
  bool decodeQuotedEscape(std::string &output);

  Token lexSlash();
  Token lexDot();
  Token lexEqual();
  Token lexBang();
  Token lexLess();
  Token lexGreater();
  Token lexPlus();
  Token lexMinus();
  Token lexStar();
  Token lexPercent();
  Token lexAmpersand();
  Token lexPipe();
  Token lexCaret();
  Token lexTilde();
  Token lexQuestion();
  Token lexHash();

  static bool isDigitForBase(char ch, unsigned base);
  static unsigned digitValue(char ch);
  static bool isAsciiIdentifierStart(char ch);
  static bool isAsciiIdentifierContinue(char ch);

  static std::string decodeIdentifier(std::string_view text);
  static std::string_view stripNumericSeparators(std::string_view text);

  void finalizeInteger(Token &token, unsigned base);
  void finalizeFloat(Token &token);
};

} // namespace hyperlang

#endif
