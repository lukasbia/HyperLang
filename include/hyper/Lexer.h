#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace hyper {

enum class TokenKind {
    EndOfFile,
    Unknown,
    Identifier,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    CharacterLiteral,

    KwIf,
    KwElse,
    KwWhile,
    KwFor,
    KwIn,
    KwFunc,
    KwVar,
    KwLet,
    KwConst,
    KwReturn,
    KwStruct,
    KwClass,
    KwEnum,
    KwProtocol,
    KwImport,
    KwModule,
    KwPublic,
    KwPrivate,
    KwInternal,
    KwStatic,
    KwInit,
    KwDeinit,
    KwDefer,
    KwBreak,
    KwContinue,
    KwTrue,
    KwFalse,
    KwNil,
    KwAnd,
    KwOr,
    KwNot,
    KwMove,
    KwBorrow,
    KwOwn,
    KwWeak,
    KwAuto,
    KwType,
    KwInt,
    KwFloat,
    KwBool,
    KwString,

    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Equal,
    EqualEqual,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    PlusEqual,
    MinusEqual,
    StarEqual,
    SlashEqual,
    PercentEqual,
    Ampersand,
    Pipe,
    Caret,
    Bang,
    Tilde,
    Dot,
    Comma,
    Colon,
    Semicolon,
    Arrow,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket
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

class Lexer {
public:
    explicit Lexer(std::string_view source);

    Token next();
    std::vector<Token> tokenize();
    bool atEnd() const noexcept;

private:
    std::string_view source_;
    std::size_t current_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    char peek() const noexcept;
    char peekNext() const noexcept;
    char advance() noexcept;
    bool match(char expected) noexcept;
    void skipWhitespaceAndComments();
    Token identifierOrKeyword();
    Token number();
    Token string();
    Token character();
    Token makeToken(TokenKind kind, std::size_t start, SourceLocation location) const;
};

const char *tokenKindName(TokenKind kind) noexcept;

} // namespace hyper
