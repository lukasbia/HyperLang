#pragma once

#include <cstddef>
#include <string>

namespace hyperlang::lexer {

enum class TokenKind {
    EndOfFile,
    Identifier,
    IntegerLiteral,
    NumberLiteral,
    StringLiteral,
    AtDirective,
    LBrace,
    RBrace,
    LParen,
    RParen,
    LBracket,
    RBracket,
    Dot,
    Comma,
    Colon,
    Semicolon,
    Equal,
    EqualEqual,
    NotEqual,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    AndAnd,
    OrOr,
    Bang,
    Unknown,

    KeywordInclude,
    KeywordImport,
    KeywordFunc,
    KeywordElse,
    KeywordIf,
    KeywordThen,
    KeywordEndIf,
    KeywordWhile,
    KeywordTrue,
    KeywordFalse,
    KeywordDo,
    KeywordLoop,
    KeywordEndLoop,
    KeywordLet,
    KeywordVar,
    KeywordString,
    KeywordPanic,
    KeywordInput,
    KeywordOutput,
    KeywordInit,
    KeywordDeinit,
    KeywordInt,
    KeywordNum,
    KeywordEnum,
    KeywordNil,

    KeywordClass,
    KeywordStruct,
    KeywordProtocol,
    KeywordExtension,
    KeywordTypealias,
    KeywordGuard,
    KeywordSwitch,
    KeywordCase,
    KeywordDefault,
    KeywordFor,
    KeywordIn,
    KeywordBreak,
    KeywordContinue,
    KeywordReturn,
    KeywordDefer,
    KeywordThrow,
    KeywordThrows,
    KeywordCatch,
    KeywordAsync,
    KeywordAwait,
    KeywordSome,
    KeywordAny,
    KeywordSelf
};

struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

struct Token {
    TokenKind kind = TokenKind::Unknown;
    std::string text;
    SourceLocation location;
};

const char* tokenKindName(TokenKind kind);

} // namespace hyperlang::lexer
