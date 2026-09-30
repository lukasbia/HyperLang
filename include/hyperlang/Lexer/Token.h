#pragma once
#include <cstddef>
#include <string>
namespace hyperlang::lexer {
enum class TokenKind {
 EndOfFile, Identifier, IntegerLiteral, NumberLiteral, StringLiteral, AtDirective, HashSymbol,
 LBrace, RBrace, LParen, RParen, LBracket, RBracket, Dot, Comma, Colon, Semicolon,
 Equal, EqualEqual, NotEqual, Plus, Minus, Star, Slash, Percent, Less, LessEqual, Greater, GreaterEqual,
 AndAnd, OrOr, Bang, Arrow, Unknown, AtInclude, AtImport,
 Func, Else, If, Then, EndIf, While, True, False, Do, Loop, EndLoop, Let, Var, String, Panic, Input, Output,
 Init, Deinit, Int, Num, Enum, Nil, Class, Struct, Protocol, Extension, Typealias, Guard, Switch, Case,
 Default, For, In, Break, Continue, Return, Defer, Throw, Throws, Catch, Async, Await, Some, Any, Self,
 Where, Get, Set, Mutating, Static, Final, Private, Public, Internal, Operator, Subscript,
 AssociatedType, Required, Convenience, Override, Weak, Unowned, Borrow, Consume, Yield, Macro, Attribute
};
struct SourceLocation { std::size_t offset=0; std::size_t line=1; std::size_t column=1; };
struct Token { TokenKind kind=TokenKind::Unknown; std::string text; SourceLocation location; };
const char* tokenKindName(TokenKind kind);
} // namespace hyperlang::lexer
