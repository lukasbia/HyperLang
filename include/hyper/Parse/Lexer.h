#pragma once

#include "hyper/Parse/LexerOptions.h"
#include "hyper/Parse/LexerToken.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace hyper {

struct LexerDiagnostic {
    enum class Severity { Note, Warning, Error };

    Severity severity = Severity::Error;
    SourceLocation location;
    std::string message;
    std::string replacement;
    std::size_t offset = 0;
    std::size_t length = 0;
};

class Lexer {
public:
    explicit Lexer(std::string_view source);

    Token next();
    Token peek();
    Token lookahead(std::size_t distance);
    std::vector<Token> tokenize();

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

    const std::vector<LexerDiagnostic>& diagnostics() const noexcept;
    void clearDiagnostics();

    static bool isIdentifier(std::string_view text);
    static bool isOperator(std::string_view text);
    static bool isValidEscapedIdentifier(std::string_view text);
    static bool isEscapedIdentifierEntirelyWhitespace(std::string_view text);

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

    char peekCharacter(std::size_t distance = 0) const noexcept;
    char advance() noexcept;
    bool match(char expected) noexcept;
    bool startsWith(std::string_view text) const noexcept;
    SourceLocation locationFromOffset(std::size_t offset) const;
    Token makeToken(TokenKind kind, std::size_t start,
                    SourceLocation location, bool malformed = false) const;

    void diagnose(LexerDiagnostic::Severity severity, SourceLocation location,
                  std::string message, std::string replacement = {});
    void diagnoseError(SourceLocation location, std::string message,
                       std::string replacement = {});
    void diagnoseWarning(SourceLocation location, std::string message,
                         std::string replacement = {});

    bool isWhitespaceAt(std::size_t offset) const noexcept;
    bool isLineBreakAt(std::size_t offset) const noexcept;
    bool lexLineComment();
    bool lexBlockComment();
    bool lexHashbang();
    bool lexConflictMarker();
    bool lexCommentTrivia();
    void consumeTrivia();

    Token lexIdentifierOrKeyword();
    Token lexEscapedIdentifier();
    Token lexDollarIdentifier();

    bool scanDecimalDigits(std::size_t& position, bool allowUnderscores,
                           std::size_t& count, bool& hadSeparators);
    bool scanBasedDigits(std::size_t& position, NumberBase base,
                         std::size_t& count, bool& hadSeparators);
    Token lexBinaryNumber();
    Token lexOctalNumber();
    Token lexHexNumber();
    Token lexDecimalNumber();
    Token lexNumber();

    bool scanUnicodeEscape(std::size_t& position, std::uint32_t& value);
    bool scanEscapeSequence(std::size_t& position, std::string& decoded,
                            bool& valid);
    bool scanStringDelimiter(std::size_t& delimiterLength, bool& multiline);
    bool scanInterpolatedExpression(std::size_t openingOffset,
                                    std::size_t& closingOffset);
    Token lexString();
    Token lexRawString(unsigned delimiterLength);
    Token lexCharacter();
    Token lexDirective();
    Token lexHashConstruct();
    static bool looksLikeEditorPlaceholder(std::string_view text);
    static bool looksLikeDirective(std::string_view text);
    static bool looksLikeConflictMarker(std::string_view text);
    Token lexRegex();
    Token lexOperator();
    OperatorBinding classifyOperatorBinding(std::string_view source,
                                            std::size_t start,
                                            std::size_t end);
    Token lexUnknown();

    void invalidateLookahead();

    static bool isASCIIIdentifierStart(char value) noexcept;
    static bool isASCIIIdentifierContinue(char value) noexcept;
    static bool isASCIIDigit(char value) noexcept;
    static bool isASCIIHexDigit(char value) noexcept;
    static bool isASCIIOctalDigit(char value) noexcept;
    static bool isASCIIWhitespace(char value) noexcept;
    static bool isPrintableASCII(char value) noexcept;
    static bool isUnicodeIdentifierStart(std::uint32_t value) noexcept;
    static bool isUnicodeIdentifierContinue(std::uint32_t value) noexcept;
    static bool isRawIdentifierWhitespace(std::uint32_t value) noexcept;
    static bool isForbiddenIdentifierCodePoint(std::uint32_t value) noexcept;
    static bool isOperatorCharacter(char value) noexcept;
    static bool isOperatorStartCharacter(char value) noexcept;
    static bool isOperatorContinuationCharacter(char value) noexcept;
    static bool encodeUTF8(std::uint32_t codePoint, std::string& output);
    static std::uint32_t validateUTF8Character(std::string_view bytes,
                                               std::size_t& offset);

    static TokenKind keywordKind(std::string_view text) noexcept;
    static TokenKind punctuationKind(std::string_view text) noexcept;
    static TokenKind operatorKind(std::string_view text) noexcept;
};

const char* tokenKindName(TokenKind kind) noexcept;

} // namespace hyper
