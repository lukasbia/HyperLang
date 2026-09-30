#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "hyper/Parse/LexerCharacter.h"
#include "hyper/Parse/LexerOptions.h"
#include "hyper/Parse/LexerSupport.h"
#include "hyper/Parse/LexerToken.h"

namespace hyper {

class Lexer {
public:
    explicit Lexer(std::string_view source);

    Token next();
    Token peek();
    Token lookahead(std::size_t distance);
    std::vector<Token> tokenize();

    void reset();
    void setOptions(const LexerOptions& options);
    const LexerOptions& options() const noexcept;

    bool atEnd() const noexcept;
    std::size_t currentOffset() const noexcept;
    SourceLocation currentLocation() const noexcept;

    const std::vector<LexerDiagnostic>& diagnostics() const noexcept;
    void clearDiagnostics();

private:
    std::string_view source_;
    std::size_t current_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;
    LexerOptions options_{};
    std::vector<LexerDiagnostic> diagnostics_;
    std::vector<Token> lookaheadBuffer_;

    char peekCharacter(std::size_t distance = 0) const noexcept;
    char advanceCharacter() noexcept;
    bool matchCharacter(char character) noexcept;
    bool startsWith(std::string_view text) const noexcept;

    Token lexIdentifierOrKeyword();
    Token lexNumber();
    Token lexString();
    Token lexCharacter();
    Token lexRegex();
    Token lexDirective();
    Token lexOperator();
    Token lexUnknown();

    void consumeTrivia();
    bool consumeLineComment();
    bool consumeBlockComment();
    bool consumeHashbang();

    void diagnose(
        LexerDiagnosticSeverity severity,
        SourceLocation location,
        std::string message
    );
};

const char* tokenKindName(
    TokenKind kind
) noexcept;

} // namespace hyper
