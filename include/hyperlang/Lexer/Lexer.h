#pragma once

#include "hyperlang/Lexer/Token.h"

#include <string_view>
#include <vector>

namespace hyperlang::lexer {

class Lexer {
public:
    explicit Lexer(std::string_view source);

    std::vector<Token> tokenize();

    const std::vector<Token>& tokens() const noexcept;

    std::size_t offset() const noexcept;
    SourceLocation location() const noexcept;

private:
    std::string_view source_;
    std::size_t index_ = 0;
    SourceLocation location_{};
    std::vector<Token> tokens_;

    char peek(std::size_t distance = 0) const noexcept;
    char advance() noexcept;
    bool atEnd() const noexcept;
    bool match(char expected) noexcept;

    void skipWhitespace();
    void skipLineComment();
    void skipBlockComment();

    Token lexIdentifierOrKeyword();
    Token lexNumber();
    Token lexString();
    Token lexCharacter();
    Token lexDirective();
    Token lexHashDirective();
    Token lexOperatorOrPunctuation();

    Token makeToken(TokenKind kind, SourceLocation start) const;
    Token makeToken(TokenKind kind, SourceLocation start, std::size_t begin) const;

    static bool isIdentifierStart(char character) noexcept;
    static bool isIdentifierContinue(char character) noexcept;
    static bool isDecimalDigit(char character) noexcept;
    static bool isHexDigit(char character) noexcept;
    static int hexadecimalValue(char character) noexcept;
};

} // namespace hyperlang::lexer
