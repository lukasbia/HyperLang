#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_OPTIONS_H
#define HYPER_LIB_PARSE_LEXER_OPTIONS_H
#include <cstdint>
namespace hyper::parse {
enum class LexerMode : std::uint8_t { HyperLang, Interface, HIL };
enum class CommentRetentionMode : std::uint8_t { Skip, Preserve };
enum class RegexLexingMode : std::uint8_t { Disabled, Tentative, Enabled };
struct LexerOptions {
    LexerMode mode = LexerMode::HyperLang;
    CommentRetentionMode commentRetention = CommentRetentionMode::Skip;
    RegexLexingMode regexMode = RegexLexingMode::Disabled;
    bool preserveComments = false;
    bool allowUnicodeIdentifiers = true;
    bool allowNumericSeparators = true;
    bool allowBinaryLiterals = true;
    bool allowOctalLiterals = true;
    bool allowHexLiterals = true;
    bool allowScientificNotation = true;
    bool diagnoseUnknownUnicode = false;
    bool allowHashbang = false;
    bool enableCodeCompletion = false;
};
}
#endif
