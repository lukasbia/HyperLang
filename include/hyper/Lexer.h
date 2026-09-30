#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "hyper/LexerOptions.h"

namespace hyper {

struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

enum class TokenKind : std::uint16_t {
    EndOfFile, Unknown, Identifier, EscapedIdentifier, DollarIdentifier,
    IntegerLiteral, BinaryIntegerLiteral, OctalIntegerLiteral, HexIntegerLiteral,
    FloatLiteral, HexFloatLiteral, StringLiteral, MultilineStringLiteral, CharacterLiteral,
    RegexLiteral, EditorPlaceholder, AtSign, Hash, Directive,
    KwIf, KwElse, KwWhile, KwDo, KwFor, KwIn, KwFunc, KwVar, KwLet, KwConst, KwReturn,
    KwStruct, KwClass, KwEnum, KwProtocol, KwExtension, KwImport, KwModule, KwPackage, KwNamespace,
    KwPublic, KwPrivate, KwInternal, KwProtected, KwStatic, KwFinal, KwOverride, KwInit, KwDeinit, KwDefer,
    KwBreak, KwContinue, KwTrue, KwFalse, KwNil, KwAnd, KwOr, KwNot, KwMove, KwBorrow, KwConsume, KwCopy,
    KwOwn, KwShared, KwWeak, KwUnowned, KwAuto, KwType, KwTypealias, KwAssociatedType, KwInt, KwFloat, KwBool,
    KwString, KwBytes, KwNum, KwGuard, KwSwitch, KwCase, KwDefault, KwThrow, KwThrows, KwCatch, KwAsync, KwAwait,
    KwTask, KwActor, KwSome, KwAny, KwSelf, KwWhere, KwGet, KwSet, KwMutating, KwOperator, KwSubscript, KwYield,
    KwMacro, KwAttribute, KwObserve, KwSynchronize, KwCompile, KwExtern, KwOutput, KwInput, KwPanic, KwLoop,
    KwEndLoop, KwThen, KwEndIf, KwSource, KwFile, KwFunction, KwProperty, KwEvent, KwSignal, KwDetached,
    KwIsolated, KwNonisolated, KwSendable,
    Plus, Minus, Star, Slash, Percent, Ampersand, Pipe, Caret, Tilde, Bang, Question,
    Equal, EqualEqual, NotEqual, Greater, GreaterEqual, Less, LessEqual,
    PlusEqual, MinusEqual, StarEqual, SlashEqual, PercentEqual, AmpersandEqual, PipeEqual, CaretEqual,
    Arrow, FatArrow, Range, ClosedRange, NilCoalescing, Power, ShiftLeft, ShiftRight, ShiftLeftEqual, ShiftRightEqual,
    AndAnd, OrOr, QuestionQuestion, TildeEqual, Dot, Comma, Colon, DoubleColon, Semicolon, Backslash,
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket,
    Comment, DocComment, Hashbang, ConflictMarker
};

enum class OperatorBinding : std::uint8_t {
    None, Prefix, Postfix, BinarySpaced, BinaryUnspaced
};

enum class NumberBase : std::uint8_t {
    Decimal, Binary, Octal, Hexadecimal
};

enum class StringKind : std::uint8_t {
    Normal, Multiline, Raw, RawMultiline
};

struct NumericLiteralInfo {
    NumberBase base = NumberBase::Decimal;
    bool hasDecimalPoint = false;
    bool hasExponent = false;
    bool exponentIsBinary = false;
    bool hasSeparators = false;
    bool hasLeadingZero = false;
    bool hasSuffix = false;
};

struct StringLiteralInfo {
    StringKind kind = StringKind::Normal;
    unsigned customDelimiterLength = 0;
    bool hasInterpolation = false;
    bool hasEscapes = false;
    bool terminated = true;
};

struct Token {
    TokenKind kind = TokenKind::Unknown;
    std::string text;
    SourceLocation location{};
    OperatorBinding binding = OperatorBinding::None;
    NumericLiteralInfo numeric{};
    StringLiteralInfo string{};
    bool atStartOfLine = false;
    bool escapedIdentifier = false;
    bool malformed = false;
    bool hasLeadingComment = false;

    std::size_t endOffset() const noexcept {
        return location.offset + text.size();
    }
};

struct LexerDiagnostic {
    enum class Severity { Note, Warning, Error };
    Severity severity = Severity::Error;
    SourceLocation location{};
    std::string message;
    std::string replacement;
};

class Lexer {
public:
    explicit Lexer(std::string_view source);

    Token next();
    std::vector<Token> tokenize();
    Token peek();
    Token lookahead(std::size_t distance);

    void reset();
    void setOptions(const LexerOptions& options);
    const LexerOptions& options() const noexcept;

    void setKeepComments(bool);
    void setAllowHashbang(bool);
    void setAllowRegexLiterals(bool);
    void setTreatEditorPlaceholdersAsTokens(bool);

    bool keepComments() const noexcept;
    bool allowHashbang() const noexcept;
    bool allowRegexLiterals() const noexcept;
    bool atEnd() const noexcept;

    std::size_t currentOffset() const noexcept;
    SourceLocation currentLocation() const noexcept;
    const std::vector<LexerDiagnostic>& diagnostics() const noexcept;
    void clearDiagnostics();

    static bool isIdentifier(std::string_view);
    static bool isOperator(std::string_view);
    static bool isValidEscapedIdentifier(std::string_view);
    static std::uint32_t validateUTF8Character(std::string_view, std::size_t&);
    static bool encodeUTF8(std::uint32_t, std::string&);

private:
    std::string_view source_;
    std::size_t current_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    LexerOptions options_{};
    bool keepComments_ = false;
    bool allowHashbang_ = true;
    bool allowRegexLiterals_ = false;
    bool treatEditorPlaceholdersAsTokens_ = true;
    bool atStartOfLine_ = true;
    bool hadLeadingComment_ = false;

    std::vector<LexerDiagnostic> diagnostics_;
    std::vector<Token> lookaheadBuffer_;

    char peek(std::size_t = 0) const noexcept;
    char advance() noexcept;
    bool match(char) noexcept;
    bool startsWith(std::string_view) const noexcept;

    SourceLocation locationFromOffset(std::size_t) const;
    Token makeToken(TokenKind, std::size_t, SourceLocation, bool = false) const;

    Token lexIdentifierOrKeyword();
    Token lexEscapedIdentifier();
    Token lexDollarIdentifier();
    Token lexNumber();
    Token lexString();
    Token lexCharacter();
    Token lexRawString(unsigned);
    Token lexRegex();
    Token lexDirective();
    Token lexHashConstruct();
    Token lexOperator();
    Token lexUnknown();

    bool lexLineComment();
    bool lexBlockComment();
    bool lexHashbang();
    bool lexConflictMarker();

    bool scanInterpolatedExpression(std::size_t, std::size_t&);
    bool scanStringDelimiter(std::size_t&, bool&);
    bool scanUnicodeEscape(std::size_t&, std::uint32_t&);
    bool scanEscapeSequence(std::size_t&, std::string&, bool&);
    bool scanDecimalDigits(std::size_t&, bool, std::size_t&, bool&);
    bool scanBasedDigits(std::size_t&, NumberBase, std::size_t&, bool&);

    bool isWhitespaceAt(std::size_t) const noexcept;
    bool isLineBreakAt(std::size_t) const noexcept;

    void diagnose(LexerDiagnostic::Severity, SourceLocation, std::string, std::string = {});
    void diagnoseError(SourceLocation, std::string, std::string = {});
    void diagnoseWarning(SourceLocation, std::string, std::string = {});
    void updateLineState(char) noexcept;
    void consumeTrivia();
    void invalidateLookahead();

    static bool isASCIIIdentifierStart(char) noexcept;
    static bool isASCIIIdentifierContinue(char) noexcept;
    static bool isASCIIDigit(char) noexcept;
    static bool isASCIIHexDigit(char) noexcept;
    static bool isASCIIOctalDigit(char) noexcept;
    static bool isASCIIWhitespace(char) noexcept;
    static bool isPrintableASCII(char) noexcept;
    static bool isUnicodeIdentifierStart(std::uint32_t) noexcept;
    static bool isUnicodeIdentifierContinue(std::uint32_t) noexcept;
    static bool isForbiddenIdentifierCodePoint(std::uint32_t) noexcept;
    static bool isOperatorCharacter(char) noexcept;
    static bool isOperatorStartCharacter(char) noexcept;
    static bool isOperatorContinuationCharacter(char) noexcept;
    static TokenKind keywordKind(std::string_view) noexcept;
    static TokenKind punctuationKind(std::string_view) noexcept;
    static TokenKind operatorKind(std::string_view) noexcept;
    static OperatorBinding classifyOperatorBinding(std::string_view, std::size_t, std::size_t) noexcept;
    static bool looksLikeEditorPlaceholder(std::string_view) noexcept;
    static bool looksLikeDirective(std::string_view) noexcept;
    static bool looksLikeConflictMarker(std::string_view) noexcept;
};

const char* tokenKindName(TokenKind) noexcept;

} // namespace hyper
