#pragma once
#include <cstdint>

namespace hyper {

enum class TokenKind : std::uint16_t {
    KwIf, KwElse, KwWhile, KwDo, KwFor, KwIn, KwFunc, KwVar, KwLet, KwConst,
    KwReturn, KwStruct, KwClass, KwEnum, KwProtocol, KwExtension, KwImport,
    KwModule, KwPackage, KwNamespace, KwPublic, KwPrivate, KwInternal, KwProtected,
    KwStatic, KwFinal, KwOverride, KwInit, KwDeinit, KwDefer, KwBreak, KwContinue,
    KwTrue, KwFalse, KwNil, KwAnd, KwOr, KwNot, KwMove, KwBorrow, KwConsume, KwCopy,
    KwOwn, KwShared, KwWeak, KwUnowned, KwAuto, KwType, KwTypealias, KwAssociatedType,
    KwInt, KwFloat, KwBool, KwString, KwBytes, KwNum, KwGuard, KwSwitch, KwCase,
    KwDefault, KwThrow, KwThrows, KwCatch, KwAsync, KwAwait, KwTask, KwActor, KwSome,
    KwAny, KwSelf, KwWhere, KwGet, KwSet, KwMutating, KwOperator, KwSubscript,
    KwYield, KwMacro, KwAttribute, KwObserve, KwSynchronize, KwCompile, KwExtern,
    KwOutput, KwInput, KwPanic, KwLoop, KwEndLoop, KwThen, KwEndIf, KwSource, KwFile,
    KwFunction, KwProperty, KwEvent, KwSignal, KwDetached, KwIsolated, KwNonisolated,
    KwSendable,
    Plus, Minus, Star, Slash, Percent, Ampersand, Pipe, Caret, Tilde, Bang, Question,
    Equal, EqualEqual, NotEqual, Greater, GreaterEqual, Less, LessEqual, PlusEqual,
    MinusEqual, StarEqual, SlashEqual, PercentEqual, AmpersandEqual, PipeEqual,
    CaretEqual, Arrow, FatArrow, Range, ClosedRange, NilCoalescing, Power, ShiftLeft,
    ShiftRight, ShiftLeftEqual, ShiftRightEqual, AndAnd, OrOr, TildeEqual, DoubleColon,
    DocComment, Comment, Hashbang, ConflictMarker, Identifier, Unknown, EscapedIdentifier,
    DollarIdentifier, BinaryIntegerLiteral, OctalIntegerLiteral, HexIntegerLiteral,
    HexFloatLiteral, FloatLiteral, IntegerLiteral, MultilineStringLiteral, StringLiteral,
    Hash, CharacterLiteral, AtSign, Directive, RegexLiteral, EndOfFile, EditorPlaceholder,
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket, Comma,
    Semicolon, Colon, Backslash, Dot, QuestionQuestion, Or
};

} // namespace hyper
