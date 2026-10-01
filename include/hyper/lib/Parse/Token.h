#pragma once
#ifndef HYPER_LIB_PARSE_TOKEN_H
#define HYPER_LIB_PARSE_TOKEN_H
#include "hyper/lib/Parse/SourceLocation.h"
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
namespace hyper::parse {
enum class TokenKind : std::uint16_t {
    Eof,
    Unknown,
    Identifier,
    Keyword,
    SpecialKeyword,
    IntegerLiteral,
    FloatingLiteral,
    StringLiteral,
    CharacterLiteral,
    TrueLiteral,
    FalseLiteral,
    NilLiteral,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Comma,
    Dot,
    Colon,
    Semicolon,
    Question,
    At,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Ampersand,
    Pipe,
    Caret,
    Tilde,
    Bang,
    Equal,
    Less,
    Greater,
    PlusEqual,
    MinusEqual,
    StarEqual,
    SlashEqual,
    PercentEqual,
    AmpEqual,
    PipeEqual,
    CaretEqual,
    EqualEqual,
    BangEqual,
    LessEqual,
    GreaterEqual,
    AmpAmp,
    PipePipe,
    Arrow,
    FatArrow,
    Increment,
    Decrement,
    ShiftLeft,
    ShiftRight,
    ShiftLeftEqual,
    ShiftRightEqual,
    Range,
    RangeInclusive,
    NullCoalescing,
    Scope,
    Ellipsis,
    DoubleColon,
    QuestionDot,
    BangDot,
    AtApi,
    AtApiId,
    AtFile,
    AtFilename,
    AtFileId,
    AtAudio,
    AtPlayAudio,
    AtImage,
    AtImageId
};
std::string_view tokenKindName(TokenKind) noexcept;
bool isKeyword(TokenKind) noexcept; bool isSpecialKeyword(TokenKind) noexcept; bool isLiteral(TokenKind) noexcept; bool isPunctuation(TokenKind) noexcept; bool isOperator(TokenKind) noexcept;
struct Token {
 TokenKind kind=TokenKind::Unknown; SourceRange range{}; std::string text{};
 Token()=default; Token(TokenKind k,SourceRange r,std::string t):kind(k),range(r),text(std::move(t)){}
 bool is(TokenKind k)const noexcept{return kind==k;} bool valid()const noexcept{return kind!=TokenKind::Unknown;}
 bool isEOF()const noexcept{return kind==TokenKind::Eof;} bool isIdentifier()const noexcept{return kind==TokenKind::Identifier;}
 bool isKeyword()const noexcept{return hyper::parse::isKeyword(kind);} bool isSpecialKeyword()const noexcept{return hyper::parse::isSpecialKeyword(kind);}
 std::string_view spelling()const noexcept{return text;} SourceOffset start()const noexcept{return range.begin.offset;} SourceOffset end()const noexcept{return range.end.offset;}
};
}
#endif
