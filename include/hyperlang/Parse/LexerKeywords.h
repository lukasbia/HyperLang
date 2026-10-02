//===--- LexerKeywords.h - HyperLang Lexer Keyword Tables ----------------===//

#ifndef HYPERLANG_PARSE_LEXER_KEYWORDS_H
#define HYPERLANG_PARSE_LEXER_KEYWORDS_H

#include "hyperlang/Parse/TokenKinds.h"

#include <array>
#include <cstdint>
#include <string_view>
#include <utility>

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

inline constexpr auto KeywordTable = std::to_array<KeywordEntry>({
  {"struct", tok::Kind::KwStruct},
  {"import", tok::Kind::KwImport},
  {"pass", tok::Kind::KwPass},
  {"self", tok::Kind::KwSelf},
  {"if", tok::Kind::KwIf},
  {"else", tok::Kind::KwElse},
  {"endif", tok::Kind::KwEndIf},
  {"close", tok::Kind::KwClose},
  {"private", tok::Kind::KwPrivate},
  {"public", tok::Kind::KwPublic},
  {"string", tok::Kind::KwString},
  {"int", tok::Kind::KwInt},
  {"init", tok::Kind::KwInit},
  {"deinit", tok::Kind::KwDeinit},
  {"enum", tok::Kind::KwEnum},
  {"num", tok::Kind::KwNum},
  {"var", tok::Kind::KwVar},
  {"const", tok::Kind::KwConst},
  {"data", tok::Kind::KwData},
  {"async", tok::Kind::KwAsync},
  {"nil", tok::Kind::KwNil},
  {"while", tok::Kind::KwWhile},
  {"loop", tok::Kind::KwLoop},
  {"do", tok::Kind::KwDo},
  {"get", tok::Kind::KwGet},
  {"getData", tok::Kind::KwGetData},
  {"then", tok::Kind::KwThen},
  {"funct", tok::Kind::KwFunct},
  {"destroy", tok::Kind::KwDestroy},
  {"delete", tok::Kind::KwDelete},
  {"click", tok::Kind::KwClick},
  {"when", tok::Kind::KwWhen},
  {"alpha", tok::Kind::KwAlpha},
  {"borrow", tok::Kind::KwBorrow},
  {"free", tok::Kind::KwFree},
  {"open", tok::Kind::KwOpen},
  {"send", tok::Kind::KwSend},
  {"message", tok::Kind::KwMessage},
  {"move", tok::Kind::KwMove},
  {"format", tok::Kind::KwFormat},
  {"case", tok::Kind::KwCase},
  {"class", tok::Kind::KwClass},
  {"output", tok::Kind::KwOutput},
  {"return", tok::Kind::KwReturn},
  {"for", tok::Kind::KwFor},
  {"in", tok::Kind::KwIn},
  {"break", tok::Kind::KwBreak},
  {"continue", tok::Kind::KwContinue},
  {"func", tok::Kind::KwFunc},
  {"main", tok::Kind::KwMain},
  {"let", tok::Kind::KwLet},
  {"true", tok::Kind::KwTrue},
  {"false", tok::Kind::KwFalse},
  {"default", tok::Kind::KwDefault},
  {"switch", tok::Kind::KwSwitch},
  {"protocol", tok::Kind::KwProtocol},
  {"extension", tok::Kind::KwExtension},
  {"type", tok::Kind::KwType},
  {"typealias", tok::Kind::KwTypealias},
  {"static", tok::Kind::KwStatic},
  {"internal", tok::Kind::KwInternal},
  {"guard", tok::Kind::KwGuard},
  {"defer", tok::Kind::KwDefer},
  {"await", tok::Kind::KwAwait},
  {"throw", tok::Kind::KwThrow},
  {"throws", tok::Kind::KwThrows},
  {"try", tok::Kind::KwTry},
  {"catch", tok::Kind::KwCatch},
  {"task", tok::Kind::KwTask},
  {"repeat", tok::Kind::KwRepeat},
  {"and", tok::Kind::KwAnd},
  {"or", tok::Kind::KwOr},
  {"not", tok::Kind::KwNot},
  {"until", tok::Kind::KwUntil},
  {"where", tok::Kind::KwWhere},
  {"with", tok::Kind::KwWith},
  {"as", tok::Kind::KwAs},
  {"is", tok::Kind::KwIs},
  {"error", tok::Kind::KwError},
  {"panic", tok::Kind::KwPanic},
  {"bool", tok::Kind::KwBool},
  {"float", tok::Kind::KwFloat},
  {"double", tok::Kind::KwDouble},
  {"bytes", tok::Kind::KwBytes},
  {"range", tok::Kind::KwRange},
  {"array", tok::Kind::KwArray},
  {"map", tok::Kind::KwMap},
  {"set", tok::Kind::KwSet},
  {"tuple", tok::Kind::KwTuple},
  {"binary", tok::Kind::KwBinary},
  {"os", tok::Kind::KwOS},
  {"kernel", tok::Kind::KwKernel},
  {"file", tok::Kind::KwFile},
  {"fileName", tok::Kind::KwFileName},
  {"section", tok::Kind::KwSection},
  {"connect", tok::Kind::KwConnect},
  {"backup", tok::Kind::KwBackup},
  {"createData", tok::Kind::KwCreateData},
  {"control", tok::Kind::KwControl},
  {"change", tok::Kind::KwChange},
  {"use", tok::Kind::KwUse},
  {"placeholder", tok::Kind::KwPlaceholder},
  {"sink", tok::Kind::KwSink},
  {"play", tok::Kind::KwPlay},
});

inline constexpr auto AttributeTable = std::to_array<AttributeEntry>({
  {"@file", tok::Kind::AtFile},
  {"@fileID", tok::Kind::AtFileID},
  {"@api", tok::Kind::AtAPI},
  {"@apiID", tok::Kind::AtAPIID},
  {"@audio", tok::Kind::AtAudio},
  {"@playAudio", tok::Kind::AtPlayAudio},
  {"@manualMemoryManagement", tok::Kind::AtManualMemoryManagement},
  {"#keyBoard", tok::Kind::HashKeyboard},
  {"#keyBoardKey", tok::Kind::HashKeyboardKey},
  {"@import", tok::Kind::AtImport},
  {"@repo", tok::Kind::AtRepo},
  {"@webLink", tok::Kind::AtWebLink},
  {"@database", tok::Kind::AtDatabase},
  {"@target", tok::Kind::AtTarget},
  {"@available", tok::Kind::AtAvailable},
  {"@main", tok::Kind::AtMain},
  {"@test", tok::Kind::AtTest},
});constexpr bool isKeywordSpelling(std::string_view spelling) noexcept {
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
    if (entry.hash == hash && entry.spelling == spelling)
      return entry.kind;
  return tok::Kind::Identifier;
}

} // namespace hyperlang::lexer

#endif
