#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace hyper {
struct SourceLocation { std::size_t offset=0; std::size_t line=1; std::size_t column=1; };
enum class OperatorBinding : std::uint8_t { None, Prefix, Postfix, Binary };
enum class NumberBase : std::uint8_t { Decimal, Binary, Octal, Hexadecimal };
enum class StringKind : std::uint8_t { Normal, Multiline, Raw, Character, Regex };

enum class TokenKind : std::uint16_t {
 EndOfFile,Unknown,Identifier,EscapedIdentifier,DollarIdentifier,
 IntegerLiteral,BinaryIntegerLiteral,OctalIntegerLiteral,HexIntegerLiteral,FloatLiteral,HexFloatLiteral,
 StringLiteral,MultilineStringLiteral,CharacterLiteral,RegexLiteral,EditorPlaceholder,AtSign,Hash,Directive,
 Plus,Minus,Star,Slash,Percent,Ampersand,Pipe,Caret,Tilde,Bang,Question,Equal,EqualEqual,NotEqual,
 Greater,GreaterEqual,Less,LessEqual,PlusEqual,MinusEqual,StarEqual,SlashEqual,PercentEqual,AmpersandEqual,
 PipeEqual,CaretEqual,Arrow,FatArrow,Range,ClosedRange,NilCoalescing,QuestionQuestion,Power,ShiftLeft,
 ShiftRight,ShiftLeftEqual,ShiftRightEqual,AndAnd,OrOr,TildeEqual,DoubleColon,Dot,Comma,Colon,Semicolon,
 Backslash,LeftParen,RightParen,LeftBrace,RightBrace,LeftBracket,RightBracket,Comment,DocComment,Hashbang,
 ConflictMarker,KwIf,KwElse,KwWhile,KwDo,KwFor,KwIn,KwFunc,KwVar,KwLet,KwConst,KwReturn,KwStruct,KwClass,
 KwEnum,KwProtocol,KwExtension,KwImport,KwModule,KwPackage,KwNamespace,KwPublic,KwPrivate,KwInternal,
 KwProtected,KwStatic,KwFinal,KwOverride,KwInit,KwDeinit,KwDefer,KwBreak,KwContinue,KwTrue,KwFalse,KwNil,
 KwAnd,KwOr,KwNot,KwMove,KwBorrow,KwConsume,KwCopy,KwOwn,KwShared,KwWeak,KwUnowned,KwAuto,KwType,
 KwTypealias,KwAssociatedType,KwInt,KwFloat,KwBool,KwString,KwBytes,KwNum,KwGuard,KwSwitch,KwCase,KwDefault,
 KwThrow,KwThrows,KwCatch,KwAsync,KwAwait,KwTask,KwActor,KwSome,KwAny,KwSelf,KwWhere,KwGet,KwSet,KwMutating,
 KwOperator,KwSubscript,KwYield,KwMacro,KwAttribute,KwObserve,KwSynchronize,KwCompile,KwExtern,KwOutput,
 KwInput,KwPanic,KwLoop,KwEndLoop,KwThen,KwEndIf,KwSource,KwFile,KwFunction,KwProperty,KwEvent,KwSignal,
 KwDetached,KwIsolated,KwNonisolated,KwSendable
};

struct NumericLiteralInfo {
 NumberBase base=NumberBase::Decimal; bool hasDecimalPoint=false; bool hasExponent=false;
 bool hasSeparators=false; bool hasSuffix=false; bool isInteger=false; bool isFloating=false;
};
struct StringLiteralInfo {
 StringKind kind=StringKind::Normal; unsigned customDelimiterLength=0; bool hasInterpolation=false;
 bool hasEscapes=false; bool terminated=true; bool multiline=false; bool raw=false;
};
struct Token {
 TokenKind kind=TokenKind::Unknown; std::string text; SourceLocation location;
 OperatorBinding binding=OperatorBinding::None; NumericLiteralInfo numeric; StringLiteralInfo string;
 bool atStartOfLine=false; bool escapedIdentifier=false; bool malformed=false;
 bool hasLeadingComment=false; bool hasTrailingComment=false; bool isDocumentation=false; bool isDirective=false;
 std::size_t endOffset() const noexcept { return location.offset+text.size(); }
 bool isIdentifier() const noexcept; bool isLiteral() const noexcept; bool isOperator() const noexcept;
 bool isTrivia() const noexcept; bool isKeyword() const noexcept; bool isPunctuation() const noexcept;
};
const char* tokenKindName(TokenKind) noexcept;
TokenKind operatorKind(std::string_view text);
} // namespace hyper
