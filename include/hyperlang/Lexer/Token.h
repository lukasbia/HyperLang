#pragma once
#include <cstddef>
#include <string>
namespace hyperlang::lexer {
enum class TokenKind { EndOfFile, Identifier, IntegerLiteral, NumberLiteral, StringLiteral, AtDirective, HashDirective, LBrace, RBrace, LParen, RParen, LBracket, RBracket, Dot, Comma, Colon, Semicolon, Equal, EqualEqual, NotEqual, Plus, Minus, Star, Slash, Less, LessEqual, Greater, GreaterEqual, AndAnd, OrOr, Bang, Unknown, KeywordInclude, KeywordImport, KeywordFunc, KeywordElse, KeywordIf, KeywordThen, KeywordEndIf, KeywordWhile, KeywordTrue, KeywordFalse, KeywordDo, KeywordLoop, KeywordEndLoop, KeywordLet, KeywordVar, KeywordString, KeywordPanic, KeywordInput, KeywordOutput, KeywordInit, KeywordDeinit, KeywordInt, KeywordNum, KeywordEnum, KeywordNil };
struct SourceLocation { std::size_t offset=0; std::size_t line=1; std::size_t column=1; };
struct Token { TokenKind kind=TokenKind::Unknown; std::string text; SourceLocation location; };
const char* tokenKindName(TokenKind kind);
} // namespace hyperlang::lexer
