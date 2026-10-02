#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_H
#define HYPER_LIB_PARSE_LEXER_H
#include "hyper/lib/Parse/Token.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
namespace hyper::parse {
enum class DiagnosticSeverity { Note, Warning, Error };
struct LexerDiagnostic { DiagnosticSeverity severity; SourceRange range; std::string message; };
class Lexer {
public:
    explicit Lexer(std::string_view source);
    Token lex();
    std::vector<Token> lexAll();
    Token peek(std::size_t lookahead=0);
    const std::vector<LexerDiagnostic>& diagnostics() const noexcept;
    bool hasErrors() const noexcept;
    static bool isIdentifierStart(char32_t) noexcept;
    static bool isIdentifierContinue(char32_t) noexcept;
    static bool isKeyword(std::string_view) noexcept;
private:
    struct UTF8 { char32_t value=0; std::size_t width=0; bool valid=false; };
    Token lexImpl(), make(TokenKind,std::size_t), identifier(), specialIdentifier(), number(), stringLiteral(), characterLiteral(), punctuation();
    void skipTrivia(), lineComment(), blockComment();
    bool consumeEscape(), consumeUnicodeEscape(), consumeDigits(unsigned);
    UTF8 decode(std::size_t) const noexcept;
    bool consumeIdentifierCodePoint(bool);
    bool atEnd() const noexcept; char current() const noexcept; char look(std::size_t=1) const noexcept;
    bool consume(char) noexcept;
    void diagnose(DiagnosticSeverity,std::size_t,std::size_t,std::string_view);
    std::string_view source_;
    std::size_t offset_=0;
    bool atStartOfLine_=true;
    bool leadingSpace_=false;
    std::vector<LexerDiagnostic> diagnostics_;
    std::vector<Token> lookahead_;
};
}
#endif
