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

constexpr std::uint64_t identifierHash(std::string_view value) noexcept {
  std::uint64_t hash = 0xcbf29ce484222325ULL;
  for (unsigned char ch : value) {
    hash ^= static_cast<std::uint64_t>(ch);
    hash *= 0x100000001B3ULL;
  }
  return hash;
}

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
  const std::size_t remaining = source_.size() - cursor_;
  count = std::min(count, remaining);
  while (count != 0 && static_cast<unsigned char>(source_[cursor_]) < 0x80u) {
    if (source_[cursor_] == '\\n') {
      ++line_;
      column_ = 1;
    } else {
      ++column_;
    }
    ++cursor_;
    --count;
  }
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

tok::Kind Lexer::classifyIdentifier(std::string_view spelling) {
  switch (identifierHash(spelling)) {
  case 0x86628378db071ea7ULL:
    if (spelling == "func")
      return tok::Kind::KwFunc;
    break;
  case 0x1f5962a2ce9803c8ULL:
    if (spelling == "main")
      return tok::Kind::KwMain;
    break;
  case 0x690418194ed15d3eULL:
    if (spelling == "var")
      return tok::Kind::KwVar;
    break;
  case 0x127284191dcc577aULL:
    if (spelling == "let")
      return tok::Kind::KwLet;
    break;
  case 0x65c9718e19e3df34ULL:
    if (spelling == "const")
      return tok::Kind::KwConst;
    break;
  case 0x8b73007b55c3e26ULL:
    if (spelling == "if")
      return tok::Kind::KwIf;
    break;
  case 0x7f2b6c605332dd30ULL:
    if (spelling == "else")
      return tok::Kind::KwElse;
    break;
  case 0x2579c7ef323d0d96ULL:
    if (spelling == "then")
      return tok::Kind::KwThen;
    break;
  case 0xce87a3885811296eULL:
    if (spelling == "while")
      return tok::Kind::KwWhile;
    break;
  case 0x8915907b53bb494ULL:
    if (spelling == "do")
      return tok::Kind::KwDo;
    break;
  case 0xdcb27818fed9da90ULL:
    if (spelling == "for")
      return tok::Kind::KwFor;
    break;
  case 0x8b73807b55c4bbeULL:
    if (spelling == "in")
      return tok::Kind::KwIn;
    break;
  case 0xcdece4ad70d57debULL:
    if (spelling == "loop")
      return tok::Kind::KwLoop;
    break;
  case 0x93b7591debc7ce38ULL:
    if (spelling == "break")
      return tok::Kind::KwBreak;
    break;
  case 0xd2bfd5accd1966a4ULL:
    if (spelling == "continue")
      return tok::Kind::KwContinue;
    break;
  case 0xc5c7b983377cad5fULL:
    if (spelling == "return")
      return tok::Kind::KwReturn;
    break;
  case 0xfb1abb75bac1c83bULL:
    if (spelling == "defer")
      return tok::Kind::KwDefer;
    break;
  case 0xaabe362250e8c5eeULL:
    if (spelling == "guard")
      return tok::Kind::KwGuard;
    break;
  case 0xa5a87ac5b0b379b1ULL:
    if (spelling == "switch")
      return tok::Kind::KwSwitch;
    break;
  case 0xb55e6190e8792fd1ULL:
    if (spelling == "case")
      return tok::Kind::KwCase;
    break;
  case 0xebada5168620c5feULL:
    if (spelling == "default")
      return tok::Kind::KwDefault;
    break;
  case 0x915ea6605dc15d80ULL:
    if (spelling == "enum")
      return tok::Kind::KwEnum;
    break;
  case 0xc318a9d898991720ULL:
    if (spelling == "struct")
      return tok::Kind::KwStruct;
    break;
  case 0xd11655952fcbab9fULL:
    if (spelling == "class")
      return tok::Kind::KwClass;
    break;
  case 0xb2b41998fa3bfff5ULL:
    if (spelling == "protocol")
      return tok::Kind::KwProtocol;
    break;
  case 0x1f82d98c5190e81aULL:
    if (spelling == "extension")
      return tok::Kind::KwExtension;
    break;
  case 0xa79439ef7bfa9c2dULL:
    if (spelling == "type")
      return tok::Kind::KwType;
    break;
  case 0x871b773d6ded3c8fULL:
    if (spelling == "typealias")
      return tok::Kind::KwTypealias;
    break;
  case 0xf5d2afc57ab57213ULL:
    if (spelling == "init")
      return tok::Kind::KwInit;
    break;
  case 0x8bae25483e72d928ULL:
    if (spelling == "deinit")
      return tok::Kind::KwDeinit;
    break;
  case 0xc534816d6d11e97bULL:
    if (spelling == "static")
      return tok::Kind::KwStatic;
    break;
  case 0xf4b72165f60f5b60ULL:
    if (spelling == "public")
      return tok::Kind::KwPublic;
    break;
  case 0xc5a11c2dd9ab8cecULL:
    if (spelling == "private")
      return tok::Kind::KwPrivate;
    break;
  case 0xe08a40f0f6141500ULL:
    if (spelling == "internal")
      return tok::Kind::KwInternal;
    break;
  case 0x38fe7925726e5cbdULL:
    if (spelling == "protected")
      return tok::Kind::KwProtected;
    break;
  case 0xf4d30088534a1af4ULL:
    if (spelling == "import")
      return tok::Kind::KwImport;
    break;
  case 0xc71bef1c7cd467a7ULL:
    if (spelling == "include")
      return tok::Kind::KwInclude;
    break;
  case 0x43e6a23362b6daddULL:
    if (spelling == "module")
      return tok::Kind::KwModule;
    break;
  case 0xdc1c4a04cb5c6da0ULL:
    if (spelling == "namespace")
      return tok::Kind::KwNamespace;
    break;
  case 0x53cf3eec39cf731bULL:
    if (spelling == "package")
      return tok::Kind::KwPackage;
    break;
  case 0x5cc53b62ea063f7ULL:
    if (spelling == "extern")
      return tok::Kind::KwExtern;
    break;
  case 0xe43c3b14f8fd3d4ULL:
    if (spelling == "inline")
      return tok::Kind::KwInline;
    break;
  case 0x1df91b0a2ff56ed4ULL:
    if (spelling == "noreturn")
      return tok::Kind::KwNoReturn;
    break;
  case 0x7268f9bc90cbb82fULL:
    if (spelling == "async")
      return tok::Kind::KwAsync;
    break;
  case 0x30ad89d85a5364dULL:
    if (spelling == "await")
      return tok::Kind::KwAwait;
    break;
  case 0x267436cb62108b74ULL:
    if (spelling == "throws")
      return tok::Kind::KwThrows;
    break;
  case 0x5a5fe3720c9584cfULL:
    if (spelling == "throw")
      return tok::Kind::KwThrow;
    break;
  case 0xc1b2e33b13ec076cULL:
    if (spelling == "catch")
      return tok::Kind::KwCatch;
    break;
  case 0x570ac119447423eeULL:
    if (spelling == "try")
      return tok::Kind::KwTry;
    break;
  case 0x6035dc18f0bbd4d1ULL:
    if (spelling == "some")
      return tok::Kind::KwSome;
    break;
  case 0xe6f7b419052023cdULL:
    if (spelling == "any")
      return tok::Kind::KwAny;
    break;
  case 0x2d19e518d40792b7ULL:
    if (spelling == "self")
      return tok::Kind::KwSelf;
    break;
  case 0xd0d708b40e957634ULL:
    if (spelling == "super")
      return tok::Kind::KwSuper;
    break;
  case 0x2587bcef32493841ULL:
    if (spelling == "this")
      return tok::Kind::KwThis;
    break;
  case 0x5b5c98ef514dbfa5ULL:
    if (spelling == "true")
      return tok::Kind::KwTrue;
    break;
  case 0xb5fae2c14238b978ULL:
    if (spelling == "false")
      return tok::Kind::KwFalse;
    break;
  case 0x2146ba19257dc6acULL:
    if (spelling == "nil")
      return tok::Kind::KwNil;
    break;
  case 0x5b9bc4ba528108e4ULL:
    if (spelling == "null")
      return tok::Kind::KwNull;
    break;
  case 0x629576c6b305d539ULL:
    if (spelling == "optional")
      return tok::Kind::KwOptional;
    break;
  case 0x9b51cd7cd76778c4ULL:
    if (spelling == "result")
      return tok::Kind::KwResult;
    break;
  case 0x9f7452dd75d54d31ULL:
    if (spelling == "error")
      return tok::Kind::KwError;
    break;
  case 0x5eb889a73f8bfbd4ULL:
    if (spelling == "panic")
      return tok::Kind::KwPanic;
    break;
  case 0xd0f17a2c3f687b4ULL:
    if (spelling == "move")
      return tok::Kind::KwMove;
    break;
  case 0xbf903911984eee4ULL:
    if (spelling == "copy")
      return tok::Kind::KwCopy;
    break;
  case 0x6a867a578ceffce0ULL:
    if (spelling == "borrow")
      return tok::Kind::KwBorrow;
    break;
  case 0x7e64d19174a1419ULL:
    if (spelling == "mut")
      return tok::Kind::KwMut;
    break;
  case 0x1923443d4dbc1fd7ULL:
    if (spelling == "unsafe")
      return tok::Kind::KwUnsafe;
    break;
  case 0x2584e9e765c2f420ULL:
    if (spelling == "atomic")
      return tok::Kind::KwAtomic;
    break;
  case 0x9575f08fb5a48f0dULL:
    if (spelling == "volatile")
      return tok::Kind::KwVolatile;
    break;
  case 0x7c324d8022a76537ULL:
    if (spelling == "final")
      return tok::Kind::KwFinal;
    break;
  case 0xc5726f7f4345bb1dULL:
    if (spelling == "override")
      return tok::Kind::KwOverride;
    break;
  case 0x554667f2165fec5dULL:
    if (spelling == "operator")
      return tok::Kind::KwOperator;
    break;
  case 0x379d6b8893e8fc78ULL:
    if (spelling == "where")
      return tok::Kind::KwWhere;
    break;
  case 0x9bf6bff64f32f639ULL:
    if (spelling == "when")
      return tok::Kind::KwWhen;
    break;
  case 0xa66b06f655a0c2e9ULL:
    if (spelling == "with")
      return tok::Kind::KwWith;
    break;
  case 0x89c5507b545b54dULL:
    if (spelling == "as")
      return tok::Kind::KwAs;
    break;
  case 0x8b74507b55c61d5ULL:
    if (spelling == "is")
      return tok::Kind::KwIs;
    break;
  case 0x7f845078d7a5c0b5ULL:
    if (spelling == "from")
      return tok::Kind::KwFrom;
    break;
  case 0x8c83907b56ac0a4ULL:
    if (spelling == "to")
      return tok::Kind::KwTo;
    break;
  case 0x1fcc8b4e81fbac2fULL:
    if (spelling == "until")
      return tok::Kind::KwUntil;
    break;
  case 0x8a64b07b54df8d4ULL:
    if (spelling == "by")
      return tok::Kind::KwBy;
    break;
  case 0x150f53f8651b4384ULL:
    if (spelling == "output")
      return tok::Kind::KwOutput;
    break;
  case 0x2f0792248c7d6068ULL:
    if (spelling == "print")
      return tok::Kind::KwPrint;
    break;
  case 0x855b556730a34a05ULL:
    if (spelling == "data")
      return tok::Kind::KwData;
    break;
  case 0x2e37f3011795f95fULL:
    if (spelling == "getData")
      return tok::Kind::KwGetData;
    break;
  case 0xa87d20dd9071345dULL:
    if (spelling == "createData")
      return tok::Kind::KwCreateData;
    break;
  case 0x2f2ec0474f1c4fe4ULL:
    if (spelling == "bytes")
      return tok::Kind::KwBytes;
    break;
  case 0x704be0d8faaffc58ULL:
    if (spelling == "string")
      return tok::Kind::KwString;
    break;
  case 0xcd2fd49bc6b014bdULL:
    if (spelling == "bool")
      return tok::Kind::KwBool;
    break;
  case 0x2b9fff192bd4c83eULL:
    if (spelling == "int")
      return tok::Kind::KwInt;
    break;
  case 0x394d16e46cd6fca1ULL:
    if (spelling == "uint")
      return tok::Kind::KwUInt;
    break;
  case 0xf5a67dc57a8fe232ULL:
    if (spelling == "int8")
      return tok::Kind::KwInt8;
    break;
  case 0xf9e84c8f42970271ULL:
    if (spelling == "int16")
      return tok::Kind::KwInt16;
    break;
  case 0xf9e1c08f4291a8dfULL:
    if (spelling == "int32")
      return tok::Kind::KwInt32;
    break;
  case 0xf9d0c88f42834344ULL:
    if (spelling == "int64")
      return tok::Kind::KwInt64;
    break;
  case 0x34fa7f24f14f37fbULL:
    if (spelling == "uint8")
      return tok::Kind::KwUInt8;
    break;
  case 0x54bf46c60981dbb2ULL:
    if (spelling == "uint16")
      return tok::Kind::KwUInt16;
    break;
  case 0x54c64ac60988012cULL:
    if (spelling == "uint32")
      return tok::Kind::KwUInt32;
    break;
  case 0x54d746c609966d93ULL:
    if (spelling == "uint64")
      return tok::Kind::KwUInt64;
    break;
  case 0xa00a62a942b20165ULL:
    if (spelling == "float")
      return tok::Kind::KwFloat;
    break;
  case 0xa0880a9ce131dea8ULL:
    if (spelling == "double")
      return tok::Kind::KwDouble;
    break;
  case 0x2102bb192543fb93ULL:
    if (spelling == "num")
      return tok::Kind::KwNum;
    break;
  case 0xdd4665ded410134cULL:
    if (spelling == "decimal")
      return tok::Kind::KwDecimal;
    break;
  case 0x526eb811b28d5cb2ULL:
    if (spelling == "range")
      return tok::Kind::KwRange;
    break;
  case 0x4f9e14b634c6b026ULL:
    if (spelling == "array")
      return tok::Kind::KwArray;
    break;
  case 0x80f5919176d2d91ULL:
    if (spelling == "map")
      return tok::Kind::KwMap;
    break;
  case 0x823b87195ce20e23ULL:
    if (spelling == "set")
      return tok::Kind::KwSet;
    break;
  case 0x9123e5d0c6b648d1ULL:
    if (spelling == "tuple")
      return tok::Kind::KwTuple;
    break;
  case 0xaad01178f02a6a23ULL:
    if (spelling == "file")
      return tok::Kind::KwFile;
    break;
  case 0x7acc2bd06d594688ULL:
    if (spelling == "fileName")
      return tok::Kind::KwFileName;
    break;
  case 0xa699eae1563fe6ecULL:
    if (spelling == "section")
      return tok::Kind::KwSection;
    break;
  case 0x52f8ab92f7c3fb18ULL:
    if (spelling == "kernel")
      return tok::Kind::KwKernel;
    break;
  case 0x8b05507b5565e57ULL:
    if (spelling == "os")
      return tok::Kind::KwOS;
    break;
  case 0xee885e7447d3d73cULL:
    if (spelling == "binary")
      return tok::Kind::KwBinary;
    break;
  case 0x54cb3aded715c1b9ULL:
    if (spelling == "connect")
      return tok::Kind::KwConnect;
    break;
  case 0x8f75c61dfd19093fULL:
    if (spelling == "backup")
      return tok::Kind::KwBackup;
    break;
  case 0x3b4240debc883f8ULL:
    if (spelling == "pass")
      return tok::Kind::KwPass;
    break;
  case 0xf3fe6b5fdb85d50aULL:
    if (spelling == "delete")
      return tok::Kind::KwDelete;
    break;
  case 0x2fce30cb8e3c8185ULL:
    if (spelling == "destroy")
      return tok::Kind::KwDestroy;
    break;
  case 0xf5b9f7190cc182e3ULL:
    if (spelling == "cut")
      return tok::Kind::KwCut;
    break;
  case 0xc2f00318f053500aULL:
    if (spelling == "end")
      return tok::Kind::KwEnd;
    break;
  case 0x14e5faab9ce0e362ULL:
    if (spelling == "block")
      return tok::Kind::KwBlock;
    break;
  case 0x2e75a5d2c90e3310ULL:
    if (spelling == "shrink")
      return tok::Kind::KwShrink;
    break;
  case 0x1f4f66a2ce8fb60fULL:
    if (spelling == "math")
      return tok::Kind::KwMath;
    break;
  case 0xbf4ba5ad694f5907ULL:
    if (spelling == "line")
      return tok::Kind::KwLine;
    break;
  case 0x9a7ce19baa54c278ULL:
    if (spelling == "base")
      return tok::Kind::KwBase;
    break;
  case 0x546401b5d2a8d2a4ULL:
    if (spelling == "message")
      return tok::Kind::KwMessage;
    break;
  case 0x4e2e8418e6525c72ULL:
    if (spelling == "sink")
      return tok::Kind::KwSink;
    break;
  case 0xa5e8570db713b763ULL:
    if (spelling == "play")
      return tok::Kind::KwPlay;
    break;
  case 0x84e1cb596b7e53eULL:
    if (spelling == "control")
      return tok::Kind::KwControl;
    break;
  case 0x4a6173034c7ba27dULL:
    if (spelling == "change")
      return tok::Kind::KwChange;
    break;
  case 0x4c52b0193dcce634ULL:
    if (spelling == "use")
      return tok::Kind::KwUse;
    break;
  case 0xdc520a6761fd880eULL:
    if (spelling == "dont")
      return tok::Kind::KwDont;
    break;
  case 0x607afc808d3d3611ULL:
    if (spelling == "instead")
      return tok::Kind::KwInstead;
    break;
  case 0x8b06007b5567108ULL:
    if (spelling == "of")
      return tok::Kind::KwOf;
    break;
  case 0xd5dc35dcb7e762a9ULL:
    if (spelling == "single")
      return tok::Kind::KwSingle;
    break;
  case 0x615e79d982d9f0faULL:
    if (spelling == "placeholder")
      return tok::Kind::KwPlaceholder;
    break;
  default:
    break;
  }
  return tok::Kind::Identifier;
}

bool Lexer::isKeyword(std::string_view identifier) {
  return classifyIdentifier(identifier) != tok::Kind::Identifier;
}

bool Lexer::isAttribute(std::string_view spelling) {
  switch (identifierHash(spelling)) {
  case 0x476eb8aaf772928eULL:
    return spelling == "@import";
  case 0x96213e3be0e7742dULL:
    return spelling == "@include";
  case 0x40a857687329a2b1ULL:
    return spelling == "@file";
  case 0xcf1f182ba31c9bc4ULL:
    return spelling == "@fileID";
  case 0x83c55696fe3c4db5ULL:
    return spelling == "@api";
  case 0xb812c9034153f067ULL:
    return spelling == "@repo";
  case 0xa713438fceb5d7d3ULL:
    return spelling == "@webLink";
  case 0x8368c9178df6c64aULL:
    return spelling == "@database";
  case 0xf86bacfb036060c2ULL:
    return spelling == "@target";
  case 0x12beebda00fdcbceULL:
    return spelling == "@available";
  case 0xc3e6c2d0e11b12eULL:
    return spelling == "@main";
  case 0x93141bf182305d2bULL:
    return spelling == "@test";
  case 0xdb91cf98c7795676ULL:
    return spelling == "@deprecated";
  case 0x72b0c56157a016eaULL:
    return spelling == "@availableFrom";
  case 0x1ed22c37e8d8b907ULL:
    return spelling == "@unknown";
  default:
    return false;
  }
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
      const unsigned char byte = static_cast<unsigned char>(currentByte());
      if (byte >= 0x80u)
        break;
      if (currentByte() != ' ' && currentByte() != '\\t' &&
          currentByte() != '\\n' && currentByte() != '\\r' &&
          currentByte() != '\\v' && currentByte() != '\\f')
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
