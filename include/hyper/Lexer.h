#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace hyper {

// The lexer keeps source locations with every token so later compiler stages
// can report errors without having to recover the original source position.
struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

enum class TokenKind {
    EndOfFile,
    Unknown,

    Identifier,
    EscapedIdentifier,
    DollarIdentifier,
    IntegerLiteral,
    BinaryIntegerLiteral,
    OctalIntegerLiteral,
    HexIntegerLiteral,
    FloatLiteral,
    HexFloatLiteral,
    StringLiteral,
    MultilineStringLiteral,
    CharacterLiteral,
    RegexLiteral,
    EditorPlaceholder,

    AtSign,
    Hash,
    Directive,

    KwIf, KwElse, KwWhile, KwDo, KwFor, KwIn,
    KwFunc, KwVar, KwLet, KwConst, KwReturn,
    KwStruct, KwClass, KwEnum, KwProtocol, KwExtension,
    KwImport, KwModule, KwPackage, KwNamespace,
    KwPublic, KwPrivate, KwInternal, KwProtected,
    KwStatic, KwFinal, KwOverride,
    KwInit, KwDeinit, KwDefer,
    KwBreak, KwContinue,
    KwTrue, KwFalse, KwNil,
    KwAnd, KwOr, KwNot,
    KwMove, KwBorrow, KwConsume, KwCopy,
    KwOwn, KwShared, KwWeak, KwUnowned, KwAuto,
    KwType, KwTypealias, KwAssociatedType,
    KwInt, KwFloat, KwBool, KwString, KwBytes, KwNum,
    KwGuard, KwSwitch, KwCase, KwDefault,
    KwThrow, KwThrows, KwCatch,
    KwAsync, KwAwait, KwTask, KwActor,
    KwSome, KwAny, KwSelf,
    KwWhere, KwGet, KwSet, KwMutating,
    KwOperator, KwSubscript,
    KwYield,
    KwMacro, KwAttribute,
    KwObserve, KwSynchronize, KwCompile, KwExtern,
    KwOutput, KwInput, KwPanic,
    KwLoop, KwEndLoop, KwThen, KwEndIf,
    KwSource, KwFile, KwFunction, KwProperty,
    KwEvent, KwSignal,
    KwDetached, KwIsolated, KwNonisolated, KwSendable,

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
    Question,

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
    AmpersandEqual,
    PipeEqual,
    CaretEqual,

    Arrow,
    FatArrow,
    Range,
    ClosedRange,
    NilCoalescing,
    Power,
    ShiftLeft,
    ShiftRight,
    ShiftLeftEqual,
    ShiftRightEqual,
    AndAnd,
    OrOr,
    QuestionQuestion,
    TildeEqual,

    Dot,
    Comma,
    Colon,
    DoubleColon,
    Semicolon,
    Backslash,

    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,

    Comment,
    DocComment,
    Hashbang,
    ConflictMarker
};

enum class OperatorBinding {
    None,
    Prefix,
    Postfix,
    BinarySpaced,
    BinaryUnspaced
};

enum class NumberBase : std::uint8_t {
    Decimal,
    Binary,
    Octal,
    Hexadecimal
};

enum class StringKind : std::uint8_t {
    Normal,
    Multiline,
    Raw,
    RawMultiline
};

struct NumericLiteralInfo {
    NumberBase base = NumberBase::Decimal;
    bool hasDecimalPoint = false;
    bool hasExponent = false;
    bool exponentIsBinary = false;
    bool hasSeparators = false;
};

struct StringLiteralInfo {
    StringKind kind = StringKind::Normal;
    unsigned customDelimiterLength = 0;
    bool hasInterpolation = false;
    bool hasEscapes = false;
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
    enum class Severity {
        Note,
        Warning,
        Error
    };

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
    void setKeepComments(bool keep);
    void setAllowHashbang(bool allow);
    void setAllowRegexLiterals(bool allow);
    void setTreatEditorPlaceholdersAsTokens(bool enabled);

    bool keepComments() const noexcept;
    bool allowHashbang() const noexcept;
    bool allowRegexLiterals() const noexcept;
    bool atEnd() const noexcept;

    std::size_t currentOffset() const noexcept;
    SourceLocation currentLocation() const noexcept;

    const std::vector<LexerDiagnostic> &diagnostics() const noexcept;
    void clearDiagnostics();

    static bool isIdentifier(std::string_view text);
    static bool isOperator(std::string_view text);
    static bool isValidEscapedIdentifier(std::string_view text);

    static std::uint32_t validateUTF8Character(
        std::string_view bytes,
        std::size_t &offset);

    static bool encodeUTF8(
        std::uint32_t codePoint,
        std::string &output);

private:
    std::string_view source_;
    std::size_t current_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    bool keepComments_ = false;
    bool allowHashbang_ = true;
    bool allowRegexLiterals_ = false;
    bool treatEditorPlaceholdersAsTokens_ = true;

    bool atStartOfLine_ = true;
    bool hadLeadingComment_ = false;
    std::string leadingComment_;

    std::vector<LexerDiagnostic> diagnostics_;
    std::vector<Token> lookaheadBuffer_;

    char peek(std::size_t distance = 0) const noexcept;
    char advance() noexcept;
    bool match(char expected) noexcept;
    bool startsWith(std::string_view text) const noexcept;

    SourceLocation locationFromOffset(std::size_t offset) const;
    Token makeToken(
        TokenKind kind,
        std::size_t start,
        SourceLocation location,
        bool malformed = false) const;

    Token lexIdentifierOrKeyword();
    Token lexEscapedIdentifier();
    Token lexDollarIdentifier();
    Token lexNumber();
    Token lexBinaryNumber();
    Token lexOctalNumber();
    Token lexHexNumber();
    Token lexDecimalNumber();
    Token lexString();
    Token lexCharacter();
    Token lexRawString(unsigned delimiterLength);
    Token lexRegex();
    Token lexDirective();
    Token lexHashConstruct();
    Token lexOperator();

    bool lexCommentTrivia();
    bool lexLineComment();
    bool lexBlockComment();
    bool lexHashbang();
    bool lexConflictMarker();

    bool scanInterpolatedExpression(
        std::size_t openingOffset,
        std::size_t &closingOffset);

    bool scanStringDelimiter(
        std::size_t &delimiterLength,
        bool &multiline);

    bool scanUnicodeEscape(
        std::size_t &position,
        std::uint32_t &value);

    bool scanEscapeSequence(
        std::size_t &position,
        std::string &decoded,
        bool &valid);

    bool scanDecimalDigits(
        std::size_t &position,
        bool allowUnderscores,
        std::size_t &count,
        bool &hadSeparators);

    bool scanBasedDigits(
        std::size_t &position,
        NumberBase base,
        std::size_t &count,
        bool &hadSeparators);

    bool isWhitespaceAt(std::size_t offset) const noexcept;
    bool isLineBreakAt(std::size_t offset) const noexcept;

    void diagnose(
        LexerDiagnostic::Severity severity,
        SourceLocation location,
        std::string message,
        std::string replacement = {});

    void diagnoseError(
        SourceLocation location,
        std::string message,
        std::string replacement = {});

    void diagnoseWarning(
        SourceLocation location,
        std::string message,
        std::string replacement = {});

    void updateLineState(char consumed) noexcept;

    static bool isASCIIIdentifierStart(char c) noexcept;
    static bool isASCIIIdentifierContinue(char c) noexcept;
    static bool isASCIIDigit(char c) noexcept;
    static bool isASCIIHexDigit(char c) noexcept;
    static bool isASCIIOctalDigit(char c) noexcept;
    static bool isASCIIWhitespace(char c) noexcept;
    static bool isPrintableASCII(char c) noexcept;

    static bool isUnicodeIdentifierStart(std::uint32_t c) noexcept;
    static bool isUnicodeIdentifierContinue(std::uint32_t c) noexcept;
    static bool isRawIdentifierWhitespace(std::uint32_t c) noexcept;
    static bool isForbiddenIdentifierCodePoint(std::uint32_t c) noexcept;

    static bool isOperatorCharacter(char c) noexcept;
    static bool isOperatorStartCharacter(char c) noexcept;
    static bool isOperatorContinuationCharacter(char c) noexcept;

    static TokenKind keywordKind(std::string_view text) noexcept;
    static TokenKind punctuationKind(std::string_view text) noexcept;
    static TokenKind operatorKind(std::string_view text) noexcept;
    static OperatorBinding classifyOperatorBinding(
        std::string_view source,
        std::size_t start,
        std::size_t end) noexcept;

    static bool looksLikeEditorPlaceholder(std::string_view text) noexcept;
    static bool looksLikeDirective(std::string_view text) noexcept;
    static bool looksLikeConflictMarker(std::string_view text) noexcept;

    Token lexUnknown();

    void consumeTrivia();
    void invalidateLookahead();
};

const char *tokenKindName(TokenKind kind) noexcept;

} // namespace hyper
