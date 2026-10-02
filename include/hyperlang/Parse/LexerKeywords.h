//===--- LexerKeywords.h - HyperLang Lexer Keyword Tables ----------------===//

#ifndef HYPERLANG_PARSE_LEXER_KEYWORDS_H
#define HYPERLANG_PARSE_LEXER_KEYWORDS_H

#include "hyperlang/Parse/TokenKinds.h"

#include <array>
#include <cstdint>
#include <string_view>

namespace hyperlang::lexer {

constexpr std::uint64_t identifierHash(std::string_view value) noexcept {
  std::uint64_t hash = 0xcbf29ce484222325ULL;
  for (unsigned char byte : value) {
    hash ^= static_cast<std::uint64_t>(byte);
    hash *= 0x100000001B3ULL;
  }
  return hash;
}

struct KeywordEntry {
  std::string_view spelling;
  tok::Kind kind;
  std::uint64_t hash;

  constexpr KeywordEntry(std::string_view value, tok::Kind tokenKind)
      : spelling(value), kind(tokenKind), hash(identifierHash(value)) {}
};

struct AttributeEntry {
  std::string_view spelling;
  tok::Kind kind;
};

inline constexpr std::array<KeywordEntry, 137> KeywordTable = {{
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
  {"placeholder", tok::Kind::KwPlaceholder}
}};

inline constexpr std::array<AttributeEntry, 15> AttributeTable = {{
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

constexpr bool isKeywordSpelling(std::string_view spelling) noexcept {
  const std::uint64_t hash = identifierHash(spelling);
  for (const KeywordEntry &entry : KeywordTable)
    if (entry.hash == hash && entry.spelling == spelling)
      return true;
  return false;
}

constexpr bool isAttributeSpelling(std::string_view spelling) noexcept {
  for (const AttributeEntry &entry : AttributeTable)
    if (entry.spelling == spelling)
      return true;
  return false;
}

constexpr tok::Kind classifyKeyword(std::string_view spelling) noexcept {
  const std::uint64_t hash = identifierHash(spelling);
  for (const KeywordEntry &entry : KeywordTable)
    if (identifierHash(entry.spelling) == hash && entry.spelling == spelling)
      return entry.kind;
  return tok::Kind::Identifier;
}

} // namespace hyperlang::lexer

#endif
