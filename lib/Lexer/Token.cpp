#include "hyperlang/Lexer/Token.h"

namespace hyperlang::lexer {

const char* tokenKindName(TokenKind kind) noexcept
{
    switch (kind) {
    case TokenKind::EndOfFile: return "eof";
    case TokenKind::Identifier: return "identifier";
    case TokenKind::IntegerLiteral: return "integer-literal";
    case TokenKind::NumberLiteral: return "number-literal";
    case TokenKind::StringLiteral: return "string-literal";
    case TokenKind::CharacterLiteral: return "character-literal";
    case TokenKind::Comment: return "comment";
    case TokenKind::AtDirective: return "@directive";
    case TokenKind::HashDirective: return "#directive";
    case TokenKind::LBrace: return "{";
    case TokenKind::RBrace: return "}";
    case TokenKind::LParen: return "(";
    case TokenKind::RParen: return ")";
    case TokenKind::LBracket: return "[";
    case TokenKind::RBracket: return "]";
    case TokenKind::Dot: return ".";
    case TokenKind::Comma: return ",";
    case TokenKind::Colon: return ":";
    case TokenKind::Semicolon: return ";";
    case TokenKind::Equal: return "=";
    case TokenKind::EqualEqual: return "==";
    case TokenKind::NotEqual: return "!=";
    case TokenKind::Plus: return "+";
    case TokenKind::PlusEqual: return "+=";
    case TokenKind::Minus: return "-";
    case TokenKind::MinusEqual: return "-=";
    case TokenKind::Star: return "*";
    case TokenKind::StarEqual: return "*=";
    case TokenKind::Slash: return "/";
    case TokenKind::SlashEqual: return "/=";
    case TokenKind::Percent: return "%";
    case TokenKind::PercentEqual: return "%=";
    case TokenKind::Less: return "<";
    case TokenKind::LessEqual: return "<=";
    case TokenKind::Greater: return ">";
    case TokenKind::GreaterEqual: return ">=";
    case TokenKind::And: return "and";
    case TokenKind::Or: return "or";
    case TokenKind::Not: return "not";
    case TokenKind::BitAnd: return "&";
    case TokenKind::BitOr: return "|";
    case TokenKind::Caret: return "^";
    case TokenKind::Tilde: return "~";
    case TokenKind::Question: return "?";
    case TokenKind::Arrow: return "->";
    case TokenKind::Range: return "...";
    case TokenKind::AtInclude: return "@include";
    case TokenKind::AtImport: return "@import";
    case TokenKind::AtFile: return "@file";
    case TokenKind::AtAPI: return "@api";
    case TokenKind::AtWebLink: return "@webLink";
    case TokenKind::AtDatabase: return "@database";
    case TokenKind::Func: return "func";
    case TokenKind::Else: return "else";
    case TokenKind::If: return "if";
    case TokenKind::Then: return "then";
    case TokenKind::EndIf: return "endif";
    case TokenKind::While: return "while";
    case TokenKind::True: return "true";
    case TokenKind::False: return "false";
    case TokenKind::Do: return "do";
    case TokenKind::Loop: return "loop";
    case TokenKind::EndLoop: return "endLoop";
    case TokenKind::Let: return "let";
    case TokenKind::Var: return "var";
    case TokenKind::String: return "string";
    case TokenKind::Panic: return "panic";
    case TokenKind::Input: return "input";
    case TokenKind::Output: return "output";
    case TokenKind::Init: return "init";
    case TokenKind::Deinit: return "deinit";
    case TokenKind::Int: return "int";
    case TokenKind::Num: return "num";
    case TokenKind::Enum: return "enum";
    case TokenKind::Nil: return "nil";
    case TokenKind::Return: return "return";
    case TokenKind::Guard: return "guard";
    case TokenKind::Switch: return "switch";
    case TokenKind::Case: return "case";
    case TokenKind::Default: return "default";
    case TokenKind::For: return "for";
    case TokenKind::In: return "in";
    case TokenKind::Break: return "break";
    case TokenKind::Continue: return "continue";
    case TokenKind::Defer: return "defer";
    case TokenKind::Throw: return "throw";
    case TokenKind::Throws: return "throws";
    case TokenKind::Catch: return "catch";
    case TokenKind::Async: return "async";
    case TokenKind::Await: return "await";
    case TokenKind::Some: return "some";
    case TokenKind::Any: return "any";
    case TokenKind::Self: return "self";
    case TokenKind::Where: return "where";
    case TokenKind::Get: return "get";
    case TokenKind::Set: return "set";
    case TokenKind::Mutating: return "mutating";
    case TokenKind::Static: return "static";
    case TokenKind::Final: return "final";
    case TokenKind::Private: return "private";
    case TokenKind::Public: return "public";
    case TokenKind::Internal: return "internal";
    case TokenKind::Operator: return "operator";
    case TokenKind::Subscript: return "subscript";
    case TokenKind::AssociatedType: return "associatedtype";
    case TokenKind::Required: return "required";
    case TokenKind::Convenience: return "convenience";
    case TokenKind::Override: return "override";
    case TokenKind::Weak: return "weak";
    case TokenKind::Unowned: return "unowned";
    case TokenKind::Borrow: return "borrow";
    case TokenKind::Consume: return "consume";
    case TokenKind::Yield: return "yield";
    case TokenKind::Macro: return "macro";
    case TokenKind::Attribute: return "attribute";
    case TokenKind::Module: return "module";
    case TokenKind::Package: return "package";
    case TokenKind::Namespace: return "namespace";
    case TokenKind::Source: return "source";
    case TokenKind::File: return "file";
    case TokenKind::Function: return "function";
    case TokenKind::Property: return "property";
    case TokenKind::Event: return "event";
    case TokenKind::Signal: return "signal";
    case TokenKind::AsyncLet: return "asynclet";
    case TokenKind::Actor: return "actor";
    case TokenKind::Task: return "task";
    case TokenKind::Detach: return "detach";
    case TokenKind::Isolated: return "isolated";
    case TokenKind::Nonisolated: return "nonisolated";
    case TokenKind::Sendable: return "sendable";
    case TokenKind::Move: return "move";
    case TokenKind::Copy: return "copy";
    case TokenKind::WeakRef: return "weakref";
    case TokenKind::StrongRef: return "strongref";
    case TokenKind::Own: return "own";
    case TokenKind::Shared: return "shared";
    case TokenKind::Observe: return "observe";
    case TokenKind::Synchronize: return "synchronize";
    case TokenKind::Compile: return "compile";
    case TokenKind::Extern: return "extern";
    case TokenKind::Unknown: return "unknown";
    }

    return "unknown";
}

const char* tokenKindDescription(TokenKind kind) noexcept
{
    return tokenKindName(kind);
}

bool isKeyword(TokenKind kind) noexcept
{
    return kind >= TokenKind::Func && kind <= TokenKind::Extern;
}

bool isDirective(TokenKind kind) noexcept
{
    return kind == TokenKind::AtDirective ||
           kind == TokenKind::HashDirective ||
           kind == TokenKind::AtInclude ||
           kind == TokenKind::AtImport ||
           kind == TokenKind::AtFile ||
           kind == TokenKind::AtAPI ||
           kind == TokenKind::AtWebLink ||
           kind == TokenKind::AtDatabase;
}

bool isOperator(TokenKind kind) noexcept
{
    switch (kind) {
    case TokenKind::Equal:
    case TokenKind::EqualEqual:
    case TokenKind::NotEqual:
    case TokenKind::Plus:
    case TokenKind::PlusEqual:
    case TokenKind::Minus:
    case TokenKind::MinusEqual:
    case TokenKind::Star:
    case TokenKind::StarEqual:
    case TokenKind::Slash:
    case TokenKind::SlashEqual:
    case TokenKind::Percent:
    case TokenKind::PercentEqual:
    case TokenKind::Less:
    case TokenKind::LessEqual:
    case TokenKind::Greater:
    case TokenKind::GreaterEqual:
    case TokenKind::And:
    case TokenKind::Or:
    case TokenKind::Not:
    case TokenKind::BitAnd:
    case TokenKind::BitOr:
    case TokenKind::Caret:
    case TokenKind::Tilde:
    case TokenKind::Question:
    case TokenKind::Arrow:
        return true;
    default:
        return false;
    }
}

bool isLiteral(TokenKind kind) noexcept
{
    return kind == TokenKind::IntegerLiteral ||
           kind == TokenKind::NumberLiteral ||
           kind == TokenKind::StringLiteral ||
           kind == TokenKind::CharacterLiteral ||
           kind == TokenKind::True ||
           kind == TokenKind::False ||
           kind == TokenKind::Nil;
}

} // namespace hyperlang::lexer
