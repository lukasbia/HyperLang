//===--- LexerKeywords.h - HyperLang Lexer Keyword Tables ----------------===//

#ifndef HYPERLANG_PARSE_LEXER_KEYWORDS_H
#define HYPERLANG_PARSE_LEXER_KEYWORDS_H

#include "hyperlang/Parse/TokenKinds.h"

#include <array>
#include <cstdint>
#include <string_view>

namespace hyperlang::lexer {

struct KeywordEntry {
  std::string_view spelling;
  tok::Kind kind;
};

struct AttributeEntry {
  std::string_view spelling;
  tok::Kind kind;
};

constexpr std::uint64_t identifierHash(std::string_view value) noexcept {
  std::uint64_t hash = 0xcbf29ce484222325ULL;
  for (unsigned char byte : value) {
    hash ^= static_cast<std::uint64_t>(byte);
    hash *= 0x100000001B3ULL;
  }
  return hash;
}

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

constexpr bool isAttributeSpelling(std::string_view spelling) noexcept {
  for (const AttributeEntry &entry : AttributeTable)
    if (entry.spelling == spelling)
      return true;
  return false;
}

} // namespace hyperlang::lexer

#endif
