//===--- Lexer.cpp - HyperLang Source Lexer -------------------------------===//

#include "hyperlang/Parse/Lexer.h"
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

struct KeywordEntry {
  std::string_view spelling;
  tok::Kind kind;
};

constexpr std::array<KeywordEntry, 137> Keywords = {{
  {"func", tok::Kind::KwFunc},
  {"main", tok::Kind::KwMain},
  {"var", tok::Kind::KwVar},
  {"let", tok::Kind::KwLet},
  {"const", tok::Kind::KwConst},
  {"if", tok::Kind::KwIf},
  {"else", tok::Kind::KwElse},
  {"then", tok::Kind::KwThen},
  {"while", tok::Kind::KwWhile},
  {"do", tok::Kind::KwDo},
  {"for", tok::Kind::KwFor},
  {"in", tok::Kind::KwIn},
  {"loop", tok::Kind::KwLoop},
  {"break", tok::Kind::KwBreak},
  {"continue", tok::Kind::KwContinue},
  {"return", tok::Kind::KwReturn},
  {"defer", tok::Kind::KwDefer},
  {"guard", tok::Kind::KwGuard},
  {"switch", tok::Kind::KwSwitch},
  {"case", tok::Kind::KwCase},
  {"default", tok::Kind::KwDefault},
  {"enum", tok::Kind::KwEnum},
  {"struct", tok::Kind::KwStruct},
  {"class", tok::Kind::KwClass},
  {"protocol", tok::Kind::KwProtocol},
  {"extension", tok::Kind::KwExtension},
  {"type", tok::Kind::KwType},
  {"typealias", tok::Kind::KwTypealias},
  {"init", tok::Kind::KwInit},
  {"deinit", tok::Kind::KwDeinit},
  {"static", tok::Kind::KwStatic},
  {"public", tok::Kind::KwPublic},
  {"private", tok::Kind::KwPrivate},
  {"internal", tok::Kind::KwInternal},
  {"protected", tok::Kind::KwProtected},
  {"import", tok::Kind::KwImport},
  {"include", tok::Kind::KwInclude},
  {"module", tok::Kind::KwModule},
  {"namespace", tok::Kind::KwNamespace},
  {"package", tok::Kind::KwPackage},
  {"extern", tok::Kind::KwExtern},
  {"inline", tok::Kind::KwInline},
  {"noreturn", tok::Kind::KwNoReturn},
  {"async", tok::Kind::KwAsync},
  {"await", tok::Kind::KwAwait},
  {"throws", tok::Kind::KwThrows},
  {"throw", tok::Kind::KwThrow},
  {"catch", tok::Kind::KwCatch},
  {"try", tok::Kind::KwTry},
  {"some", tok::Kind::KwSome},
  {"any", tok::Kind::KwAny},
  {"self", tok::Kind::KwSelf},
  {"super", tok::Kind::KwSuper},
  {"this", tok::Kind::KwThis},
  {"true", tok::Kind::KwTrue},
  {"false", tok::Kind::KwFalse},
  {"nil", tok::Kind::KwNil},
  {"null", tok::Kind::KwNull},
  {"optional", tok::Kind::KwOptional},
  {"result", tok::Kind::KwResult},
  {"error", tok::Kind::KwError},
  {"panic", tok::Kind::KwPanic},
  {"move", tok::Kind::KwMove},
  {"copy", tok::Kind::KwCopy},
  {"borrow", tok::Kind::KwBorrow},
  {"mut", tok::Kind::KwMut},
  {"unsafe", tok::Kind::KwUnsafe},
  {"atomic", tok::Kind::KwAtomic},
  {"volatile", tok::Kind::KwVolatile},
  {"final", tok::Kind::KwFinal},
  {"override", tok::Kind::KwOverride},
  {"operator", tok::Kind::KwOperator},
  {"where", tok::Kind::KwWhere},
  {"when", tok::Kind::KwWhen},
  {"with", tok::Kind::KwWith},
  {"as", tok::Kind::KwAs},
  {"is", tok::Kind::KwIs},
  {"from", tok::Kind::KwFrom},
  {"to", tok::Kind::KwTo},
  {"until", tok::Kind::KwUntil},
  {"by", tok::Kind::KwBy},
  {"output", tok::Kind::KwOutput},
  {"print", tok::Kind::KwPrint},
  {"data", tok::Kind::KwData},
  {"getData", tok::Kind::KwGetData},
  {"createData", tok::Kind::KwCreateData},
  {"bytes", tok::Kind::KwBytes},
  {"string", tok::Kind::KwString},
  {"bool", tok::Kind::KwBool},
  {"int", tok::Kind::KwInt},
  {"uint", tok::Kind::KwUInt},
  {"int8", tok::Kind::KwInt8},
  {"int16", tok::Kind::KwInt16},
  {"int32", tok::Kind::KwInt32},
  {"int64", tok::Kind::KwInt64},
  {"uint8", tok::Kind::KwUInt8},
  {"uint16", tok::Kind::KwUInt16},
  {"uint32", tok::Kind::KwUInt32},
  {"uint64", tok::Kind::KwUInt64},
  {"float", tok::Kind::KwFloat},
  {"double", tok::Kind::KwDouble},
  {"num", tok::Kind::KwNum},
  {"decimal", tok::Kind::KwDecimal},
  {"range", tok::Kind::KwRange},
  {"array", tok::Kind::KwArray},
  {"map", tok::Kind::KwMap},
  {"set", tok::Kind::KwSet},
  {"tuple", tok::Kind::KwTuple},
  {"file", tok::Kind::KwFile},
  {"fileName", tok::Kind::KwFileName},
  {"section", tok::Kind::KwSection},
  {"kernel", tok::Kind::KwKernel},
  {"os", tok::Kind::KwOS},
  {"binary", tok::Kind::KwBinary},
  {"connect", tok::Kind::KwConnect},
  {"backup", tok::Kind::KwBackup},
  {"pass", tok::Kind::KwPass},
  {"delete", tok::Kind::KwDelete},
  {"destroy", tok::Kind::KwDestroy},
  {"cut", tok::Kind::KwCut},
  {"end", tok::Kind::KwEnd},
  {"block", tok::Kind::KwBlock},
  {"shrink", tok::Kind::KwShrink},
  {"math", tok::Kind::KwMath},
  {"line", tok::Kind::KwLine},
  {"base", tok::Kind::KwBase},
  {"message", tok::Kind::KwMessage},
  {"sink", tok::Kind::KwSink},
  {"play", tok::Kind::KwPlay},
  {"control", tok::Kind::KwControl},
  {"change", tok::Kind::KwChange},
  {"use", tok::Kind::KwUse},
  {"dont", tok::Kind::KwDont},
  {"instead", tok::Kind::KwInstead},
  {"of", tok::Kind::KwOf},
  {"single", tok::Kind::KwSingle},
  {"placeholder", tok::Kind::KwPlaceholder},
}};

struct AttributeEntry {
  std::string_view spelling;
  tok::Kind kind;
};

constexpr std::array<AttributeEntry, 15> Attributes = {{
  {"@import", tok::Kind::AtImport},
  {"@include", tok::Kind::AtInclude},
  {"@file", tok::Kind::AtFile},
  {"@fileID", tok::Kind::AtFileID},
  {"@api", tok::Kind::AtAPI},
  {"@repo", tok::Kind::AtRepo},
  {"@webLink", tok::Kind::AtWebLink},
  {"@database", tok::Kind::AtDatabase},
  {"@target", tok::Kind::AtTarget},
  {"@available", tok::Kind::AtAvailable},
  {"@main", tok::Kind::AtMain},
  {"@test", tok::Kind::AtTest},
  {"@deprecated", tok::Kind::AtDeprecated},
  {"@availableFrom", tok::Kind::AtAvailableFrom},
  {"@unknown", tok::Kind::AtUnknown},
}};

} // namespace

Lexer::Lexer(std::string_view source, LexerOptions options)
    : source_(source), options_(options) {}

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

  const unsigned char byte = static_cast<unsigned char>(source_[cursor_++]);
  if (byte == '\n') {
    ++line_;
    column_ = 1;
  } else {
    ++column_;
  }
}

void Lexer::advanceBytes(std::size_t count) {
  while (count-- != 0)
    advanceByte();
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
    if (source_[index] == '\n') {
      ++line_;
      column_ = 1;
    } else {
      ++column_;
    }
  }
}

void Lexer::setRetainComments(bool enabled) {
  options_.retainComments = enabled;
}

tok::Kind Lexer::classifyIdentifier(std::string_view identifier) {
  for (const KeywordEntry &entry : Keywords)
    if (entry.spelling == identifier)
      return entry.kind;
  return tok::Kind::Identifier;
}

bool Lexer::isKeyword(std::string_view identifier) {
  return classifyIdentifier(identifier) != tok::Kind::Identifier;
}

bool Lexer::isAttribute(std::string_view spelling) {
  for (const AttributeEntry &entry : Attributes)
    if (entry.spelling == spelling)
      return true;
  return false;
}

bool Lexer::isOperatorCharacter(char ch) {
  switch (ch) {
  case '+': case '-': case '*': case '/': case '%':
  case '=': case '!': case '<': case '>': case '&':
  case '|': case '^': case '~': case '?':
    return true;
  default:
    return false;
  }
}

bool Lexer::isAsciiIdentifierStart(char ch) {
  return ch == '_' || (ch >= 'A' && ch <= 'Z') ||
         (ch >= 'a' && ch <= 'z');
}

bool Lexer::isAsciiIdentifierContinue(char ch) {
  return isAsciiIdentifierStart(ch) || (ch >= '0' && ch <= '9');
}

unsigned Lexer::digitValue(char ch) {
  if (ch >= '0' && ch <= '9') return static_cast<unsigned>(ch - '0');
  if (ch >= 'a' && ch <= 'f') return static_cast<unsigned>(ch - 'a') + 10u;
  if (ch >= 'A' && ch <= 'F') return static_cast<unsigned>(ch - 'A') + 10u;
  return std::numeric_limits<unsigned>::max();
}

bool Lexer::isDigitForBase(char ch, unsigned base) {
  return digitValue(ch) < base;
}

bool Lexer::scanDigits(unsigned base, bool requireDigit) {
  bool found = false;
  bool separator = false;

  while (!atEnd()) {
    if (isDigitForBase(currentByte(), base)) {
      found = true;
      separator = false;
      advanceByte();
      continue;
    }

    if (currentByte() == '_') {
      if (!found || separator || !isDigitForBase(peekByte(), base))
        break;
      separator = true;
      advanceByte();
      continue;
    }

    break;
  }

  if (requireDigit && !found)
    error(currentLocation(), "expected a digit");

  if (separator)
    error(currentLocation(), "numeric literal cannot end with a separator");

  return found && !separator;
}

bool Lexer::scanExponent() {
  if (currentByte() != 'e' && currentByte() != 'E')
    return false;

  advanceByte();
  if (currentByte() == '+' || currentByte() == '-')
    advanceByte();

  return scanDigits(10, true);
}

bool Lexer::scanFloatSuffix() {
  if (currentByte() == 'f' || currentByte() == 'F') {
    advanceByte();
    return true;
  }
  return false;
}

bool Lexer::scanIdentifier() {
  if (atEnd())
    return false;

  if (isAsciiIdentifierStart(currentByte())) {
    advanceByte();
    while (isAsciiIdentifierContinue(currentByte()))
      advanceByte();
    return true;
  }

  if (!options_.allowUnicodeIdentifiers)
    return false;

  const unicode::DecodeResult first = unicode::decode(source_, cursor_);
  if (!first.valid || !unicode::isIdentifierStart(first.codePoint))
    return false;

  advanceBytes(first.width);

  while (!atEnd()) {
    const unicode::DecodeResult next = unicode::decode(source_, cursor_);
    if (!next.valid || !unicode::isIdentifierContinue(next.codePoint))
      break;
    advanceBytes(next.width);
  }

  return true;
}

bool Lexer::scanIdentifierContinuation() {
  if (atEnd())
    return false;
  if (isAsciiIdentifierContinue(currentByte())) {
    advanceByte();
    return true;
  }

  const unicode::DecodeResult result = unicode::decode(source_, cursor_);
  if (!result.valid || !unicode::isIdentifierContinue(result.codePoint))
    return false;

  advanceBytes(result.width);
  return true;
}

Token Lexer::lexIdentifierOrKeyword() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  scanIdentifier();

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  Token token = makeToken(classifyIdentifier(spelling), start, loc);
  token.decodedText = std::string(spelling);
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
  while (isAsciiIdentifierContinue(currentByte()))
    advanceByte();

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  for (const AttributeEntry &entry : Attributes)
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

  errno = 0;
  char *end = nullptr;
  const double value = std::strtod(normalized.c_str(), &end);

  if (end != normalized.c_str() && errno != ERANGE) {
    token.floatingValue = value;
    token.hasFloatingValue = true;
  } else {
    error(token.range.start, "invalid floating-point literal");
  }
}

Token Lexer::lexNumber() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  unsigned base = 10;
  bool floating = false;

  if (currentByte() == '0') {
    const char prefix = peekByte();
    if (prefix == 'x' || prefix == 'X') {
      base = 16;
      advanceBytes(2);
      scanDigits(16, true);
      if (currentByte() == '.') {
        floating = true;
        advanceByte();
        scanDigits(16, true);
      }
      if (currentByte() == 'p' || currentByte() == 'P') {
        floating = true;
        scanExponent();
      }
    } else if (prefix == 'b' || prefix == 'B') {
      base = 2;
      advanceBytes(2);
      scanDigits(2, true);
    } else if (prefix == 'o' || prefix == 'O') {
      base = 8;
      advanceBytes(2);
      scanDigits(8, true);
    } else {
      scanDigits(10, true);
      if (currentByte() == '.' && peekByte() != '.') {
        floating = true;
        advanceByte();
        scanDigits(10, true);
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
      scanDigits(10, true);
    }
    if (currentByte() == 'e' || currentByte() == 'E') {
      floating = true;
      scanExponent();
    }
  }

  scanFloatSuffix();

  Token token = makeToken(
      floating ? tok::Kind::FloatingLiteral : tok::Kind::IntegerLiteral,
      start, loc);

  if (floating)
    finalizeFloat(token);
  else
    finalizeInteger(token, base);

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

  Token token = makeToken(tok::Kind::CharacterLiteral, start, loc);
  token.decodedText = std::move(value);
  return token;
}

bool Lexer::skipLineComment() {
  if (currentByte() == '/' && peekByte() == '/') {
    advanceBytes(2);
    while (!atEnd() && currentByte() != '\n')
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
      const unicode::DecodeResult character = unicode::decode(source_, cursor_);
      if (!character.valid || !unicode::isWhitespace(character.codePoint))
        break;
      advanceBytes(character.width);
      consumed = true;
    }

    if (skipHashbang()) {
      consumed = true;
      continue;
    }

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

Token Lexer::lexImpl() {
  skipTrivia();

  if (atEnd()) {
    Token token;
    token.kind = tok::Kind::EndOfFile;
    token.range.start = currentLocation();
    token.range.end = currentLocation();
    return token;
  }

  const char ch = currentByte();

  if (isAsciiIdentifierStart(ch) ||
      (options_.allowUnicodeIdentifiers &&
       static_cast<unsigned char>(ch) >= 0x80u))
    return lexIdentifierOrKeyword();

  if (ch == static_cast<char>(96) && options_.allowEscapedIdentifiers)
    return lexEscapedIdentifier();

  if (ch == '@')
    return lexAttributedName();

  if (ch >= '0' && ch <= '9')
    return lexNumber();

  if (ch == '"')
    return lexString();

  if (ch == static_cast<char>(39))
    return lexCharacter();

  if (isOperatorCharacter(ch) || ch == '.' || ch == ':' ||
      ch == ',' || ch == ';' || ch == '(' || ch == ')' ||
      ch == '{' || ch == '}' || ch == '[' || ch == ']' ||
      ch == '\\' || ch == '#')
    return lexOperatorOrPunctuation();

  return lexUnknown();
}

} // namespace hyperlang
