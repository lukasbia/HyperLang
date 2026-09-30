#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace hyperlang::lexer {

enum class TokenKind : std::uint16_t {
    EndOfFile,
    Identifier,
    IntegerLiteral,
    NumberLiteral,
    StringLiteral,
    CharacterLiteral,
    Comment,
    AtDirective,
    HashDirective,
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
    PlusEqual,
    Minus,
    MinusEqual,
    Star,
    StarEqual,
    Slash,
    SlashEqual,
    Percent,
    PercentEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    And,
    Or,
    Not,
    BitAnd,
    BitOr,
    Caret,
    Tilde,
    Question,
    Arrow,
    Range,
    Unknown,

    AtInclude,
    AtImport,
    AtFile,
    AtAPI,
    AtWebLink,
    AtDatabase,

    Func,
    Else,
    If,
    Then,
    EndIf,
    While,
    True,
    False,
    Do,
    Loop,
    EndLoop,
    Let,
    Var,
    String,
    Panic,
    Input,
    Output,
    Init,
    Deinit,
    Int,
    Num,
    Enum,
    Nil,
    Return,
    Guard,
    Switch,
    Case,
    Default,
    For,
    In,
    Break,
    Continue,
    Defer,
    Throw,
    Throws,
    Catch,
    Async,
    Await,
    Some,
    Any,
    Self,
    Where,
    Get,
    Set,
    Mutating,
    Static,
    Final,
    Private,
    Public,
    Internal,
    Operator,
    Subscript,
    AssociatedType,
    Required,
    Convenience,
    Override,
    Weak,
    Unowned,
    Borrow,
    Consume,
    Yield,
    Macro,
    Attribute,
    Module,
    Package,
    Namespace,
    Source,
    File,
    Function,
    Property,
    Event,
    Signal,
    AsyncLet,
    Actor,
    Task,
    Detach,
    Isolated,
    Nonisolated,
    Sendable,
    Move,
    Copy,
    WeakRef,
    StrongRef,
    Own,
    Shared,
    Observe,
    Synchronize,
    Compile,
    Extern,

    // Compatibility aliases used while the frontend is being migrated to the new token model.
    AndAnd = And,
    OrOr = Or,
    Bang = Not,
    Struct = Module,
    Protocol = Module,
    Extension = Module,
    Typealias = Module,
    Class = Module
};

struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;

    constexpr bool operator==(const SourceLocation&) const = default;
};

struct SourceRange {
    SourceLocation begin;
    SourceLocation end;
};

struct Token {
    TokenKind kind = TokenKind::Unknown;
    std::string text;
    SourceRange range;

    bool is(TokenKind expected) const noexcept {
        return kind == expected;
    }

    bool isNot(TokenKind expected) const noexcept {
        return kind != expected;
    }

    std::string_view spelling() const noexcept {
        return text;
    }
};

const char* tokenKindName(TokenKind kind) noexcept;
const char* tokenKindDescription(TokenKind kind) noexcept;
bool isKeyword(TokenKind kind) noexcept;
bool isDirective(TokenKind kind) noexcept;
bool isOperator(TokenKind kind) noexcept;
bool isLiteral(TokenKind kind) noexcept;

} // namespace hyperlang::lexer
