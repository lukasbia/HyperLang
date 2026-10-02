//===--- Lexer.cpp - HyperLang Source Lexer -------------------------------===//

#include "hyperlang/Parse/Lexer.h"
#include "hyperlang/Parse/LexerDiagnostics.h"
#include "hyperlang/Parse/LexerKeywords.h"
#include "hyperlang/Parse/LexerLiterals.h"
#include "hyperlang/Parse/LexerOperators.h"
#include "hyperlang/Parse/LexerOptions.h"
#include "hyperlang/Parse/LexerToken.h"
#include "hyperlang/Parse/LexerTrivia.h"
#include "hyperlang/Parse/LexerUnicode.h"
#include "hyperlang/Basic/UTF8.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <string>
#include <string_view>

namespace hyperlang {
namespace {

std::string hexByte(unsigned char value) {
  static constexpr char digits[] = "0123456789ABCDEF";
  std::string result = "0x";
  result += digits[value >> 4];
  result += digits[value & 0x0F];
  return result;
}


// ASCII classification used by the hot lexer paths. These predicates keep the
// source scanner small while leaving Unicode classification to LexerUnicode.

static constexpr bool isASCIIAlphaByte(unsigned char byte) noexcept {
  return (byte >= static_cast<unsigned char>('A') &&
          byte <= static_cast<unsigned char>('Z')) ||
         (byte >= static_cast<unsigned char>('a') &&
          byte <= static_cast<unsigned char>('z'));
}

static constexpr unsigned asciiDigitValue(unsigned char byte) noexcept {
  if (byte >= static_cast<unsigned char>('0') &&
      byte <= static_cast<unsigned char>('9'))
    return static_cast<unsigned>(byte - static_cast<unsigned char>('0'));

  if (byte >= static_cast<unsigned char>('A') &&
      byte <= static_cast<unsigned char>('F'))
    return 10u + static_cast<unsigned>(byte - static_cast<unsigned char>('A'));

  if (byte >= static_cast<unsigned char>('a') &&
      byte <= static_cast<unsigned char>('f'))
    return 10u + static_cast<unsigned>(byte - static_cast<unsigned char>('a'));

  return 0xFFu;
}

static constexpr bool isASCIIDecimalDigitByte(unsigned char byte) noexcept {
  return byte >= static_cast<unsigned char>('0') &&
         byte <= static_cast<unsigned char>('9');
}

static constexpr bool isASCIIHexDigitByte(unsigned char byte) noexcept {
  return asciiDigitValue(byte) < 16u;
}

static constexpr bool isASCIIIdentifierStartByte(unsigned char byte) noexcept {
  return isASCIIAlphaByte(byte) || byte == static_cast<unsigned char>('_');
}

static constexpr bool isASCIIIdentifierContinueByte(
    unsigned char byte) noexcept {
  return isASCIIIdentifierStartByte(byte) ||
         isASCIIDecimalDigitByte(byte);
}

static constexpr bool isASCIIWhitespaceByte(unsigned char byte) noexcept {
  switch (byte) {
  case '\t':
  case '\n':
  case '\v':
  case '\f':
  case '\r':
  case ' ':
    return true;
  default:
    return false;
  }
}

static constexpr bool isASCIILineBreakByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('\r') ||
         byte == static_cast<unsigned char>('\n');
}

static constexpr bool isASCIIControlByte(unsigned char byte) noexcept {
  return byte < 0x20u && !isASCIIWhitespaceByte(byte);
}

static constexpr bool isASCIIOperatorByte(unsigned char byte) noexcept {
  switch (byte) {
  case '!':
  case '%':
  case '&':
  case '*':
  case '+':
  case '-':
  case '.':
  case '/':
  case ':':
  case '<':
  case '=':
  case '>':
  case '?':
  case '^':
  case '|':
  case '~':
    return true;
  default:
    return false;
  }
}

static constexpr bool isASCIIPunctuationByte(unsigned char byte) noexcept {
  switch (byte) {
  case '!':
  case '#':
  case '%':
  case '&':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '$':
  case '`':
  case '{':
  case '|':
  case '}':
  case '~':
    return true;
  default:
    return false;
  }
}

static constexpr bool isASCIIQuoteByte(unsigned char byte) noexcept {
  return byte == 34u || byte == 39u || byte == 96u;
}

Lexer::Lexer(std::string_view source, LexerOptions options)
    : source_(source), options_(options) {
  // HyperLang source is UTF-8. A BOM is accepted only at the beginning of the
  // buffer and is treated as source metadata rather than a token.
  consumeUTF8BOM();
}

SourceLocation Lexer::currentLocation() const {
  return {cursor_, line_, column_};
}

char Lexer::currentByte() const {
  return cursor_ < source_.size() ? source_[cursor_] : '\0';
}

char Lexer::peekByte(std::size_t distance) const {
  const std::size_t index = cursor_ + distance;
  return index < source_.size() ? source_[index] : '\0';
}

void Lexer::advanceByte() {
  if (cursor_ >= source_.size())
    return;

  const unsigned char byte =
      static_cast<unsigned char>(source_[cursor_++]);

  if (byte == '\r') {
    ++line_;
    column_ = 1;
    return;
  }

  if (byte == '\n') {
    if (cursor_ < 2 || source_[cursor_ - 2] != '\r') {
      ++line_;
      column_ = 1;
    }
    return;
  }

  ++column_;
}

void Lexer::advanceBytes(std::size_t count) {
  const std::size_t remaining = source_.size() - cursor_;
  count = std::min(count, remaining);

  while (count != 0) {
    advanceByte();
    --count;
  }
}

bool Lexer::consumeIf(char ch) {
  if (currentByte() != ch)
    return false;
  advanceByte();
  return true;
}

void Lexer::emit(Diagnostic::Severity severity, SourceLocation location,
                 std::string message) {
  diagnostics_.push_back({severity, location, std::move(message)});
}

void Lexer::error(SourceLocation location, std::string message) {
  emit(Diagnostic::Severity::Error, location, std::move(message));
}

void Lexer::warning(SourceLocation location, std::string message) {
  emit(Diagnostic::Severity::Warning, location, std::move(message));
}

Token Lexer::makeToken(tok::Kind kind, std::size_t start,
                       SourceLocation startLocation) const {
  Token token;
  token.kind = kind;
  token.range.start = startLocation;
  token.range.end = currentLocation();
  token.text = source_.substr(start, cursor_ - start);
  return token;
}

Token Lexer::lex() {
  if (lookahead_) {
    Token token = std::move(*lookahead_);
    lookahead_.reset();
    return token;
  }
  return lexImpl();
}

const Token &Lexer::peek() {
  if (!lookahead_)
    lookahead_ = lexImpl();
  return *lookahead_;
}

Token Lexer::lexIf(tok::Kind kind) {
  if (peek().kind != kind)
    return {};
  return lex();
}

bool Lexer::atEnd() const {
  return cursor_ >= source_.size();
}

std::size_t Lexer::offset() const {
  return cursor_;
}

SourceLocation Lexer::location() const {
  return currentLocation();
}

const std::vector<Diagnostic> &Lexer::diagnostics() const {
  return diagnostics_;
}

bool Lexer::hasErrors() const {
  for (const Diagnostic &diagnostic : diagnostics_)
    if (diagnostic.severity == Diagnostic::Severity::Error)
      return true;
  return false;
}

void Lexer::reset(std::size_t offset) {
  cursor_ = std::min(offset, source_.size());
  line_ = 1;
  column_ = 1;
  lookahead_.reset();
  diagnostics_.clear();

  for (std::size_t index = 0; index < cursor_; ++index) {
    if (source_[index] == '\r') {
      ++line_;
      column_ = 1;
      continue;
    }
    if (source_[index] == '\n') {
      if (index == 0 || source_[index - 1] != '\r') {
        ++line_;
        column_ = 1;
      }
      continue;
    }
    ++column_;
  }
}

void Lexer::setRetainComments(bool enabled) {
  options_.retainComments = enabled;
}

tok::Kind Lexer::classifyIdentifier(std::string_view spelling) {
  return lexer::classifyKeyword(spelling);
}

bool Lexer::isKeyword(std::string_view identifier) {
  return lexer::isKeywordSpelling(identifier);
}

bool Lexer::isAttribute(std::string_view spelling) {
  return lexer::isAttributeSpelling(spelling);
}

bool Lexer::isOperatorCharacter(char ch) {
  return lexer::isOperatorCharacter(ch);
}

Token Lexer::lexIdentifierOrKeyword() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  scanIdentifier();

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  Token token = makeToken(classifyIdentifier(spelling), start, loc);
  token.decodedText = decodeIdentifier(spelling);
  return token;
}

Token Lexer::lexEscapedIdentifier() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  advanceByte();
  const std::size_t valueStart = cursor_;

  while (!atEnd() && static_cast<unsigned char>(currentByte()) != 96) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid) {
      error(currentLocation(), "invalid UTF-8 sequence in escaped identifier");
      advanceByte();
      continue;
    }
    advanceBytes(result.width);
  }

  Token token = makeToken(tok::Kind::EscapedIdentifier, start, loc);
  token.decodedText = std::string(source_.substr(valueStart, cursor_ - valueStart));

  if (!consumeIf(static_cast<char>(96)))
    error(loc, "unterminated escaped identifier");

  token.range.end = currentLocation();
  token.text = source_.substr(start, cursor_ - start);
  return token;
}

Token Lexer::lexAttributedName() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  advanceByte();
  const std::size_t nameStart = cursor_;
  while (isAsciiIdentifierContinue(currentByte()))
    advanceByte();

  if (cursor_ == nameStart)
    return makeToken(tok::Kind::At, start, loc);

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  for (const lexer::AttributeEntry &entry : lexer::AttributeTable)
    if (entry.spelling == spelling)
      return makeToken(entry.kind, start, loc);

  Token token = makeToken(tok::Kind::AttributedName, start, loc);
  token.decodedText = std::string(spelling.substr(1));
  return token;
}

void Lexer::finalizeInteger(Token &token, unsigned base) {
  std::string normalized;
  normalized.reserve(token.text.size());

  for (char ch : token.text)
    if (ch != '_')
      normalized.push_back(ch);

  std::string_view digits = normalized;
  if (base != 10 && digits.size() >= 2)
    digits.remove_prefix(2);

  std::uint64_t value = 0;
  const auto parsed = std::from_chars(
      digits.data(), digits.data() + digits.size(), value, base);

  if (parsed.ec == std::errc{}) {
    token.integerValue = value;
    token.hasIntegerValue = true;
  } else {
    error(token.range.start, "integer literal is outside the supported range");
  }
}

void Lexer::finalizeFloat(Token &token) {
  std::string normalized;
  normalized.reserve(token.text.size());

  for (char ch : token.text)
    if (ch != '_')
      normalized.push_back(ch);

  if (!normalized.empty() &&
      (normalized.back() == 'f' || normalized.back() == 'F'))
    normalized.pop_back();

  const bool hexadecimal =
      normalized.size() >= 2 &&
      normalized[0] == '0' &&
      (normalized[1] == 'x' || normalized[1] == 'X');

  const char *first = normalized.data();
  const char *last = normalized.data() + normalized.size();
  if (hexadecimal) {
    first += 2;
    if (first == last) {
      error(token.range.start, "invalid hexadecimal floating-point literal");
      return;
    }
  }

  double value = 0.0;
  const auto parsed = std::from_chars(
      first, last, value,
      hexadecimal ? std::chars_format::hex : std::chars_format::general);

  if (parsed.ec == std::errc{} && parsed.ptr == last) {
    token.floatingValue = value;
    token.hasFloatingValue = true;
    return;
  }

  if (parsed.ec == std::errc::result_out_of_range)
    error(token.range.start, "floating-point literal is outside the supported range");
  else
    error(token.range.start, "invalid floating-point literal");
}

Token Lexer::lexNumber() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  unsigned base = 10;
  bool floating = false;
  bool hexadecimal = false;

  if (currentByte() == '0') {
    const char prefix = peekByte();
    if (prefix == 'x' || prefix == 'X') {
      base = 16;
      hexadecimal = true;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(16, true);

      if (currentByte() == '.') {
        floating = true;
        advanceByte();
        scanDigits(16, false);
      }

      if (currentByte() == 'p' || currentByte() == 'P') {
        floating = true;
        scanExponent();
      } else if (floating) {
        error(loc, "hexadecimal floating-point literal requires a binary exponent");
      }

      if (cursor_ == digitsStart && !floating)
        error(loc, "expected hexadecimal digits after '0x'");
    } else if (prefix == 'b' || prefix == 'B') {
      base = 2;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(2, true);
      if (cursor_ == digitsStart)
        error(loc, "expected binary digits after '0b'");
    } else if (prefix == 'o' || prefix == 'O') {
      base = 8;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(8, true);
      if (cursor_ == digitsStart)
        error(loc, "expected octal digits after '0o'");
    } else {
      scanDigits(10, true);
      if (currentByte() == '.' && peekByte() != '.') {
        floating = true;
        advanceByte();
        scanDigits(10, false);
      }
      if (currentByte() == 'e' || currentByte() == 'E') {
        floating = true;
        scanExponent();
      }
    }
  } else {
    scanDigits(10, true);
    if (currentByte() == '.' && peekByte() != '.') {
      floating = true;
      advanceByte();
      scanDigits(10, false);
    }
    if (currentByte() == 'e' || currentByte() == 'E') {
      floating = true;
      scanExponent();
    }
  }

  const bool hasFloatSuffix = (floating || !hexadecimal) && scanFloatSuffix();
  floating = floating || hasFloatSuffix;

  const std::string_view literal = source_.substr(start, cursor_ - start);
  if (lexer::hasInvalidNumericSeparators(literal))
    error(loc, "invalid placement of numeric separator");

  if (!literal.empty() &&
      (isAsciiIdentifierContinue(currentByte()) ||
       (static_cast<unsigned char>(currentByte()) >= 0x80u &&
        lexer::isUnicodeIdentifierContinue(source_, cursor_)))) {
    error(currentLocation(), "invalid character after numeric literal");
  }

  Token token = makeToken(
      floating ? tok::Kind::FloatingLiteral : tok::Kind::IntegerLiteral,
      start, loc);

  if (floating)
    finalizeFloat(token);
  else
    finalizeInteger(token, base);

  (void)hexadecimal;
  return token;
}

bool Lexer::decodeHexEscape(std::string &output, unsigned digits) {
  std::uint32_t value = 0;

  for (unsigned index = 0; index < digits; ++index) {
    const unsigned digit = digitValue(currentByte());
    if (digit >= 16) {
      error(currentLocation(), "invalid hexadecimal escape");
      return false;
    }
    value = (value << 4) | digit;
    advanceByte();
  }

  if (!unicode::isValidScalar(value)) {
    error(currentLocation(), "escape does not name a valid Unicode scalar");
    return false;
  }

  char bytes[4]{};
  const std::size_t length = unicode::encodedLength(value);

  if (length == 1) {
    bytes[0] = static_cast<char>(value);
  } else if (length == 2) {
    bytes[0] = static_cast<char>(0xC0 | (value >> 6));
    bytes[1] = static_cast<char>(0x80 | (value & 0x3F));
  } else if (length == 3) {
    bytes[0] = static_cast<char>(0xE0 | (value >> 12));
    bytes[1] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | (value & 0x3F));
  } else {
    bytes[0] = static_cast<char>(0xF0 | (value >> 18));
    bytes[1] = static_cast<char>(0x80 | ((value >> 12) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[3] = static_cast<char>(0x80 | (value & 0x3F));
  }

  output.append(bytes, length);
  return true;
}

bool Lexer::decodeUnicodeEscape(std::string &output) {
  if (!consumeIf('{')) {
    error(currentLocation(), "expected an opening brace in Unicode escape");
    return false;
  }

  std::uint32_t value = 0;
  unsigned digits = 0;

  while (!atEnd() && currentByte() != '}') {
    const unsigned digit = digitValue(currentByte());
    if (digit >= 16 || ++digits > 6) {
      error(currentLocation(), "invalid Unicode escape");
      return false;
    }
    value = (value << 4) | digit;
    advanceByte();
  }

  if (!consumeIf('}')) {
    error(currentLocation(), "unterminated Unicode escape");
    return false;
  }

  if (digits == 0 || !unicode::isValidScalar(value)) {
    error(currentLocation(), "invalid Unicode scalar value");
    return false;
  }

  const std::size_t length = unicode::encodedLength(value);
  char bytes[4]{};

  if (length == 1) {
    bytes[0] = static_cast<char>(value);
  } else if (length == 2) {
    bytes[0] = static_cast<char>(0xC0 | (value >> 6));
    bytes[1] = static_cast<char>(0x80 | (value & 0x3F));
  } else if (length == 3) {
    bytes[0] = static_cast<char>(0xE0 | (value >> 12));
    bytes[1] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | (value & 0x3F));
  } else {
    bytes[0] = static_cast<char>(0xF0 | (value >> 18));
    bytes[1] = static_cast<char>(0x80 | ((value >> 12) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[3] = static_cast<char>(0x80 | (value & 0x3F));
  }

  output.append(bytes, length);
  return true;
}

bool Lexer::decodeQuotedEscape(std::string &output) {
  if (atEnd())
    return false;

  const char escaped = currentByte();
  advanceByte();

  switch (escaped) {
  case 'n': output.push_back('\n'); return true;
  case 'r': output.push_back('\r'); return true;
  case 't': output.push_back('\t'); return true;
  case 'b': output.push_back('\b'); return true;
  case 'f': output.push_back('\f'); return true;
  case '0': output.push_back('\0'); return true;
  case '\\': output.push_back('\\'); return true;
  case '"': output.push_back('"'); return true;
  case 39: output.push_back(39); return true;
  case 'u': return decodeUnicodeEscape(output);
  case 'x': return decodeHexEscape(output, 2);
  default:
    error(currentLocation(), "unknown escape sequence");
    output.push_back(escaped);
    return false;
  }
}

bool Lexer::decodeEscape(std::string &output) {
  if (!consumeIf('\\'))
    return false;
  return decodeQuotedEscape(output);
}

Token Lexer::lexString() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();

  std::string value;

  while (!atEnd()) {
    if (currentByte() == '"') {
      advanceByte();
      Token token = makeToken(tok::Kind::StringLiteral, start, loc);
      token.decodedText = std::move(value);
      return token;
    }

    if (currentByte() == '\\') {
      decodeEscape(value);
      continue;
    }

    if (currentByte() == '\n') {
      error(currentLocation(), "newline is not permitted in a string literal");
      break;
    }

    const unicode::DecodeResult character = unicode::decode(source_, cursor_);
    if (!character.valid) {
      error(currentLocation(), "invalid UTF-8 sequence in string literal");
      advanceByte();
      continue;
    }

    value.append(source_.substr(cursor_, character.width));
    advanceBytes(character.width);
  }

  error(loc, "unterminated string literal");
  Token token = makeToken(tok::Kind::StringLiteral, start, loc);
  token.decodedText = std::move(value);
  return token;
}

Token Lexer::lexCharacter() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();

  std::string value;

  if (currentByte() == '\\') {
    decodeEscape(value);
  } else if (!atEnd()) {
    const unicode::DecodeResult character = unicode::decode(source_, cursor_);
    if (character.valid) {
      value.assign(source_.substr(cursor_, character.width));
      advanceBytes(character.width);
    } else {
      error(currentLocation(), "invalid UTF-8 sequence in character literal");
      advanceByte();
    }
  }

  if (!consumeIf(static_cast<char>(39)))
    error(loc, "unterminated character literal");

  if (!lexer::isSingleUnicodeScalar(value))
    error(loc, "character literal must contain exactly one Unicode scalar");

  Token token = makeToken(tok::Kind::CharacterLiteral, start, loc);
  token.decodedText = std::move(value);
  return token;
}

bool Lexer::skipLineComment() {
  if (currentByte() == '/' && peekByte() == '/') {
    advanceBytes(2);
    while (!atEnd() && currentByte() != '\n' && currentByte() != '\r')
      advanceByte();
    return true;
  }

  return false;
}

bool Lexer::skipBlockComment() {
  if (currentByte() != '/' || peekByte() != '*')
    return false;

  const SourceLocation loc = currentLocation();
  advanceBytes(2);
  unsigned depth = 1;

  while (!atEnd()) {
    if (options_.allowNestedBlockComments &&
        currentByte() == '/' && peekByte() == '*') {
      ++depth;
      advanceBytes(2);
      continue;
    }

    if (currentByte() == '*' && peekByte() == '/') {
      advanceBytes(2);
      if (--depth == 0)
        return true;
      continue;
    }

    advanceByte();
  }

  error(loc, "unterminated block comment");
  return true;
}

bool Lexer::skipHashbang() {
  if (!options_.allowHashbang || cursor_ != 0 ||
      currentByte() != '#' || peekByte() != '!')
    return false;

  while (!atEnd() && currentByte() != '\n')
    advanceByte();

  return true;
}

bool Lexer::skipTrivia() {
  bool consumed = false;

  for (;;) {
    while (!atEnd()) {
      const unsigned char byte = static_cast<unsigned char>(currentByte());
      if (byte >= 0x80u)
        break;
      if (!isASCIIWhitespaceByte(byte))
        break;
      advanceByte();
      consumed = true;
    }

    if (!atEnd() && static_cast<unsigned char>(currentByte()) >= 0x80u) {
      const unicode::DecodeResult character = unicode::decode(source_, cursor_);
      if (character.valid && unicode::isWhitespace(character.codePoint)) {
        advanceBytes(character.width);
        consumed = true;
        continue;
      }
    }

    if (skipHashbang()) {
      consumed = true;
      continue;
    }
    if (options_.retainComments && lexer::isCommentStart(source_, cursor_))
      break;
    if (skipLineComment()) {
      consumed = true;
      continue;
    }
    if (skipBlockComment()) {
      consumed = true;
      continue;
    }
    break;
  }
  return consumed;
}

bool Lexer::isAtLineStart() const {
  return column_ == 1;
}

bool Lexer::consumeUTF8BOM() {
  if (cursor_ != 0 || source_.size() < 3)
    return false;
  if (static_cast<unsigned char>(source_[0]) != 0xEF ||
      static_cast<unsigned char>(source_[1]) != 0xBB ||
      static_cast<unsigned char>(source_[2]) != 0xBF)
    return false;
  cursor_ = 3;
  line_ = 1;
  column_ = 1;
  return true;
}

bool Lexer::scanUTF8CodePoint() {
  if (atEnd())
    return false;
  const unicode::DecodeResult result = unicode::decode(source_, cursor_);
  if (!result.valid) {
    const SourceLocation loc = currentLocation();
    const unsigned char byte = static_cast<unsigned char>(currentByte());
    error(loc, "invalid UTF-8 byte " + hexByte(byte));
    advanceByte();
    return false;
  }
  advanceBytes(result.width);
  return true;
}

bool Lexer::skipHorizontalWhitespace() {
  bool consumed = false;
  for (;;) {
    if (atEnd())
      break;
    const unsigned char byte = static_cast<unsigned char>(currentByte());
    if (byte < 0x80u) {
      if (isASCIIWhitespaceByte(byte) && !isASCIILineBreakByte(byte)) {
        advanceByte();
        consumed = true;
        continue;
      }
      break;
    }
    if (!lexer::isUnicodeWhitespace(source_, cursor_))
      break;
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    advanceBytes(result.width);
    consumed = true;
  }
  return consumed;
}

Token Lexer::makeEOFToken() const {
  Token token;
  token.kind = tok::Kind::EndOfFile;
  token.range.start = currentLocation();
  token.range.end = currentLocation();
  return token;
}

Token Lexer::lexComment() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  if (currentByte() == '/' && peekByte() == '/') {
    advanceBytes(2);
    while (!atEnd() && currentByte() != '\n' && currentByte() != '\r')
      advanceByte();
    Token token = makeToken(tok::Kind::Comment, start, loc);
    token.decodedText = std::string(source_.substr(start + 2,
                                             cursor_ - start - 2));
    return token;
  }

  advanceBytes(2);
  unsigned depth = 1;
  while (!atEnd()) {
    if (options_.allowNestedBlockComments &&
        currentByte() == '/' && peekByte() == '*') {
      ++depth;
      advanceBytes(2);
      continue;
    }
    if (currentByte() == '*' && peekByte() == '/') {
      advanceBytes(2);
      if (--depth == 0)
        break;
      continue;
    }
    if (!scanUTF8CodePoint())
      continue;
  }

  if (depth != 0)
    error(loc, "unterminated block comment");

  const std::size_t contentStart = start + 2;
  const std::size_t contentEnd =
      cursor_ >= 2 && source_[cursor_ - 2] == '*' &&
      source_[cursor_ - 1] == '/' ? cursor_ - 2 : cursor_;
  Token token = makeToken(tok::Kind::Comment, start, loc);
  token.decodedText = std::string(
      source_.substr(contentStart, contentEnd - contentStart));
  return token;
}
Token Lexer::lexSlash() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::SlashEqual, start, loc);
  return makeToken(tok::Kind::Slash, start, loc);
}

Token Lexer::lexDot() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('.')) {
    if (consumeIf('<')) return makeToken(tok::Kind::DotDotLess, start, loc);
    return makeToken(tok::Kind::DotDot, start, loc);
  }
  return makeToken(tok::Kind::Dot, start, loc);
}

Token Lexer::lexEqual() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::EqualEqual, start, loc);
  if (consumeIf('>')) return makeToken(tok::Kind::FatArrow, start, loc);
  return makeToken(tok::Kind::Equal, start, loc);
}

Token Lexer::lexBang() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::NotEqual, start, loc);
  return makeToken(tok::Kind::Bang, start, loc);
}

Token Lexer::lexLess() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::LessEqual, start, loc);
  if (consumeIf('<')) {
    if (consumeIf('=')) return makeToken(tok::Kind::ShiftLeftEqual, start, loc);
    return makeToken(tok::Kind::ShiftLeft, start, loc);
  }
  return makeToken(tok::Kind::Less, start, loc);
}

Token Lexer::lexGreater() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::GreaterEqual, start, loc);
  if (consumeIf('>')) {
    if (consumeIf('=')) return makeToken(tok::Kind::ShiftRightEqual, start, loc);
    return makeToken(tok::Kind::ShiftRight, start, loc);
  }
  return makeToken(tok::Kind::Greater, start, loc);
}

Token Lexer::lexPlus() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('+')) return makeToken(tok::Kind::Increment, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::PlusEqual, start, loc);
  return makeToken(tok::Kind::Plus, start, loc);
}

Token Lexer::lexMinus() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('-')) return makeToken(tok::Kind::Decrement, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::MinusEqual, start, loc);
  if (consumeIf('>')) return makeToken(tok::Kind::Arrow, start, loc);
  return makeToken(tok::Kind::Minus, start, loc);
}

Token Lexer::lexStar() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::StarEqual, start, loc);
  return makeToken(tok::Kind::Star, start, loc);
}

Token Lexer::lexPercent() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::PercentEqual, start, loc);
  return makeToken(tok::Kind::Percent, start, loc);
}

Token Lexer::lexAmpersand() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('&')) return makeToken(tok::Kind::AmpersandAmpersand, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::AmpersandEqual, start, loc);
  return makeToken(tok::Kind::Ampersand, start, loc);
}

Token Lexer::lexPipe() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('|')) return makeToken(tok::Kind::PipePipe, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::PipeEqual, start, loc);
  return makeToken(tok::Kind::Pipe, start, loc);
}

Token Lexer::lexCaret() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::CaretEqual, start, loc);
  return makeToken(tok::Kind::Caret, start, loc);
}

Token Lexer::lexTilde() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::TildeEqual, start, loc);
  return makeToken(tok::Kind::Tilde, start, loc);
}

Token Lexer::lexQuestion() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('?')) return makeToken(tok::Kind::QuestionQuestion, start, loc);
  return makeToken(tok::Kind::Question, start, loc);
}

Token Lexer::lexHash() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  return makeToken(tok::Kind::Hash, start, loc);
}


Token Lexer::lexOperatorOrPunctuation() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  switch (currentByte()) {
  case '+': return lexPlus();
  case '-': return lexMinus();
  case '*': return lexStar();
  case '/': return lexSlash();
  case '%': return lexPercent();
  case '=': return lexEqual();
  case '!': return lexBang();
  case '<': return lexLess();
  case '>': return lexGreater();
  case '&': return lexAmpersand();
  case '|': return lexPipe();
  case '^': return lexCaret();
  case '~': return lexTilde();
  case '?': return lexQuestion();
  case '.': return lexDot();
  case '#': return lexHash();
  case ':':
    advanceByte();
    if (consumeIf(':')) return makeToken(tok::Kind::DoubleColon, start, loc);
    return makeToken(tok::Kind::Colon, start, loc);
  case '(':
    advanceByte(); return makeToken(tok::Kind::OpenParen, start, loc);
  case ')':
    advanceByte(); return makeToken(tok::Kind::CloseParen, start, loc);
  case '{':
    advanceByte(); return makeToken(tok::Kind::OpenBrace, start, loc);
  case '}':
    advanceByte(); return makeToken(tok::Kind::CloseBrace, start, loc);
  case '[':
    advanceByte(); return makeToken(tok::Kind::OpenBracket, start, loc);
  case ']':
    advanceByte(); return makeToken(tok::Kind::CloseBracket, start, loc);
  case ',':
    advanceByte(); return makeToken(tok::Kind::Comma, start, loc);
  case ';':
    advanceByte(); return makeToken(tok::Kind::Semicolon, start, loc);
  case '\\':
    advanceByte(); return makeToken(tok::Kind::Backslash, start, loc);
  case '$':
    advanceByte(); return makeToken(tok::Kind::Dollar, start, loc);
  case '`':
    advanceByte(); return makeToken(tok::Kind::Backtick, start, loc);
  default:
    advanceByte();
    return makeToken(tok::Kind::UnknownOperator, start, loc);
  }
}

Token Lexer::lexUnknown() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  const unicode::DecodeResult result = unicode::decode(source_, cursor_);

  if (!result.valid) {
    if (options_.diagnoseUnknownCharacters)
      error(loc, "invalid UTF-8 byte in source");
    advanceByte();
    return makeToken(tok::Kind::Unknown, start, loc);
  }

  if (options_.diagnoseUnknownCharacters)
    error(loc, "unexpected character in HyperLang source");

  advanceBytes(result.width);
  return makeToken(tok::Kind::Unknown, start, loc);
}


bool Lexer::scanIdentifier() {
  if (!scanIdentifierContinuation())
    return false;

  while (!atEnd()) {
    if (isAsciiIdentifierContinue(currentByte())) {
      advanceByte();
      continue;
    }

    if (static_cast<unsigned char>(currentByte()) < 0x80u)
      break;

    if (!options_.allowUnicodeIdentifiers)
      break;

    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid || !unicode::isIdentifierContinue(result.codePoint))
      break;

    advanceBytes(result.width);
  }

  return true;
}

bool Lexer::scanIdentifierContinuation() {
  if (atEnd())
    return false;

  if (isAsciiIdentifierStart(currentByte())) {
    advanceByte();
    return true;
  }

  if (options_.allowUnicodeIdentifiers &&
      static_cast<unsigned char>(currentByte()) >= 0x80u) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (result.valid && unicode::isIdentifierStart(result.codePoint)) {
      advanceBytes(result.width);
      return true;
    }
  }

  return false;
}

bool Lexer::scanDigits(unsigned base, bool requireDigit) {
  const std::size_t start = cursor_;
  bool previousWasDigit = false;

  while (!atEnd()) {
    const char ch = currentByte();
    if (isDigitForBase(ch, base)) {
      advanceByte();
      previousWasDigit = true;
      continue;
    }

    if (ch == '_') {
      if (!previousWasDigit || !isDigitForBase(peekByte(), base))
        break;
      advanceByte();
      previousWasDigit = false;
      continue;
    }

    break;
  }

  if (requireDigit && cursor_ == start)
    return false;
  return cursor_ != start;
}

bool Lexer::scanExponent() {
  if (currentByte() != 'e' && currentByte() != 'E' &&
      currentByte() != 'p' && currentByte() != 'P')
    return false;

  advanceByte();
  if (currentByte() == '+' || currentByte() == '-')
    advanceByte();

  const std::size_t digitStart = cursor_;
  scanDigits(10, true);
  if (cursor_ == digitStart) {
    error(currentLocation(), "expected exponent digits");
    return false;
  }
  return true;
}

bool Lexer::scanFloatSuffix() {
  if (currentByte() != 'f' && currentByte() != 'F')
    return false;
  if (isAsciiIdentifierContinue(peekByte()))
    return false;
  advanceByte();
  return true;
}

bool Lexer::isDigitForBase(char ch, unsigned base) {
  const unsigned value = asciiDigitValue(static_cast<unsigned char>(ch));
  return value < base;
}

unsigned Lexer::digitValue(char ch) {
  return asciiDigitValue(static_cast<unsigned char>(ch));
}

bool Lexer::isAsciiIdentifierStart(char ch) {
  return isASCIIIdentifierStartByte(static_cast<unsigned char>(ch));
}

bool Lexer::isAsciiIdentifierContinue(char ch) {
  return isASCIIIdentifierContinueByte(static_cast<unsigned char>(ch));
}

std::string Lexer::decodeIdentifier(std::string_view text) {
  return std::string(text);
}


static bool validateASCIIByteForSource(unsigned char byte) noexcept {
  if (byte == 0)
    return false;
  if (byte >= 0x80u)
    return true;
  return !isASCIIControlByte(byte) || isASCIIWhitespaceByte(byte);
}

static bool isPotentialIdentifierContinuation(std::string_view source,
                                              std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIIdentifierContinueByte(byte);

  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierContinue(result.codePoint);
}

static bool isPotentialIdentifierStart(std::string_view source,
                                       std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIIdentifierStartByte(byte);

  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierStart(result.codePoint);
}

static bool isNumericContinuation(std::string_view source,
                                  std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIDecimalDigitByte(byte) || byte == '_';

  return false;
}

static bool isOperatorLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIOperatorByte(byte);
}

static bool isQuoteLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIQuoteByte(byte);
}

static bool isPunctuationLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIPunctuationByte(byte);
}

Token Lexer::lexImpl() {
  skipTrivia();

  if (atEnd())
    return makeEOFToken();

  if (options_.retainComments && lexer::isCommentStart(source_, cursor_))
    return lexComment();

  const std::size_t tokenOffset = cursor_;
  const SourceLocation tokenLocation = currentLocation();
  const unsigned char byte = static_cast<unsigned char>(currentByte());

  switch (currentByte()) {
  case '\0':
    error(tokenLocation, "embedded null character in source");
    advanceByte();
    return makeToken(tok::Kind::Unknown, tokenOffset, tokenLocation);
  case '@':
    return lexAttributedName();
  case '`':
    if (options_.allowEscapedIdentifiers)
      return lexEscapedIdentifier();
    advanceByte();
    return makeToken(tok::Kind::Backtick, tokenOffset, tokenLocation);
  case '"':
    return lexString();
  case '\'':
    return lexCharacter();
  default:
    break;
  }

  if (isASCIIDigit(currentByte()))
    return lexNumber();

  if (isAsciiIdentifierStart(currentByte()))
    return lexIdentifierOrKeyword();

  if (options_.allowUnicodeIdentifiers && byte >= 0x80u) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid) {
      error(tokenLocation, "invalid UTF-8 sequence in source");
      advanceByte();
      return makeToken(tok::Kind::Unknown, tokenOffset, tokenLocation);
    }
    if (unicode::isIdentifierStart(result.codePoint))
      return lexIdentifierOrKeyword();
  }

  if (byte < 0x80u && isASCIIPunctuationByte(byte))
    return lexOperatorOrPunctuation();

  return lexUnknown();
}

} // namespace hyperlang