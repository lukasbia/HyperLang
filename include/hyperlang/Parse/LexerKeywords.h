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
  {"function", tok::Kind::KwFunction},
  {"throw", tok::Kind::KwThrow},
  {"bool", tok::Kind::KwBool},
  {"errorMessage", tok::Kind::KwErrorMessage},
  {"range", tok::Kind::KwRange},
  {"array", tok::Kind::KwArray},
  {"map", tok::Kind::KwMap},
  {"set", tok::Kind::KwSet},
  {"bina", tok::Kind::KwBina},
  {"os", tok::Kind::KwOS},
  {"kernel", tok::Kind::KwKernel},
  {"backup", tok::Kind::KwBackup},
  {"control", tok::Kind::KwControl},
  {"createData", tok::Kind::KwCreateData},
  {"use", tok::Kind::KwUse},
  {"placeholder", tok::Kind::KwPlaceholder},
  {"sink", tok::Kind::KwSink},
  {"play", tok::Kind::KwPlay},
  {"or", tok::Kind::KwOr},
  {"not", tok::Kind::KwNot},
  {"until", tok::Kind::KwUntil},
  {"defer", tok::Kind::KwDefer},
  {"extension", tok::Kind::KwExtension},
  {"await", tok::Kind::KwAwait},
  {"protocol", tok::Kind::KwProtocol},
  {"default", tok::Kind::KwDefault},
  {"return", tok::Kind::KwReturn},
  {"main", tok::Kind::KwMain},
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
});

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
    if (entry.hash == hash && entry.spelling == spelling)
      return entry.kind;
  return tok::Kind::Identifier;
}

} // namespace hyperlang::lexer

#endif
