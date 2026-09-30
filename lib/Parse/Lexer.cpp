#include "hyper/Lexer.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

namespace hyper {

namespace {

const std::unordered_map<std::string_view, TokenKind> keywords = {
    {"if", TokenKind::KwIf}, {"else", TokenKind::KwElse},
    {"while", TokenKind::KwWhile}, {"for", TokenKind::KwFor},
    {"in", TokenKind::KwIn}, {"func", TokenKind::KwFunc},
    {"var", TokenKind::KwVar}, {"let", TokenKind::KwLet},
    {"const", TokenKind::KwConst}, {"return", TokenKind::KwReturn},
    {"struct", TokenKind::KwStruct}, {"class", TokenKind::KwClass},
    {"enum", TokenKind::KwEnum}, {"protocol", TokenKind::KwProtocol},
    {"import", TokenKind::KwImport}, {"module", TokenKind::KwModule},
    {"public", TokenKind::KwPublic}, {"private", TokenKind::KwPrivate},
    {"internal", TokenKind::KwInternal}, {"static", TokenKind::KwStatic},
    {"init", TokenKind::KwInit}, {"deinit", TokenKind::KwDeinit},
    {"defer", TokenKind::KwDefer}, {"break", TokenKind::KwBreak},
    {"continue", TokenKind::KwContinue}, {"true", TokenKind::KwTrue},
    {"false", TokenKind::KwFalse}, {"nil", TokenKind::KwNil},
    {"and", TokenKind::KwAnd}, {"or", TokenKind::KwOr},
    {"not", TokenKind::KwNot}, {"move", TokenKind::KwMove},
    {"borrow", TokenKind::KwBorrow}, {"own", TokenKind::KwOwn},
    {"weak", TokenKind::KwWeak}, {"auto", TokenKind::KwAuto},
    {"type", TokenKind::KwType}, {"int", TokenKind::KwInt},
    {"float", TokenKind::KwFloat}, {"bool", TokenKind::KwBool},
    {"string", TokenKind::KwString}
};

bool isIdentifierStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool isIdentifierPart(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

} // namespace

Lexer::Lexer(std::string_view source) : source_(source) {}

char Lexer::peek() const noexcept {
    return current_ < source_.size() ? source_[current_] : '\0';
}

char Lexer::peekNext() const noexcept {
    return current_ + 1 < source_.size() ? source_[current_ + 1] : '\0';
}

char Lexer::advance() noexcept {
    const char c = peek();
    if (c == '\0') return c;
    ++current_;
    if (c == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }
    return c;
}

bool Lexer::match(char expected) noexcept {
    if (peek() != expected) return false;
    advance();
    return true;
}

void Lexer::skipWhitespaceAndComments() {
    for (;;) {
        while (std::isspace(static_cast<unsigned char>(peek()))) advance();

        if (peek() == '/' && peekNext() == '/') {
            while (peek() != '\n' && peek() != '\0') advance();
            continue;
        }

        if (peek() == '/' && peekNext() == '*') {
            advance();
            advance();
            while (peek() != '\0' && !(peek() == '*' && peekNext() == '/'))
                advance();
            if (peek() != '\0') {
                advance();
                advance();
            }
            continue;
        }
        return;
    }
}

Token Lexer::makeToken(TokenKind kind, std::size_t start,
                       SourceLocation location) const {
    return {kind, std::string(source_.substr(start, current_ - start)), location};
}

Token Lexer::identifierOrKeyword() {
    const std::size_t start = current_;
    const SourceLocation location{current_, line_, column_};
    while (isIdentifierPart(peek())) advance();

    const std::string_view text = source_.substr(start, current_ - start);
    const auto found = keywords.find(text);
    return makeToken(found == keywords.end() ? TokenKind::Identifier : found->second,
                     start, location);
}

Token Lexer::number() {
    const std::size_t start = current_;
    const SourceLocation location{current_, line_, column_};
    while (std::isdigit(static_cast<unsigned char>(peek()))) advance();

    TokenKind kind = TokenKind::IntegerLiteral;
    if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peekNext()))) {
        kind = TokenKind::FloatLiteral;
        advance();
        while (std::isdigit(static_cast<unsigned char>(peek()))) advance();
    }
    return makeToken(kind, start, location);
}

Token Lexer::string() {
    const std::size_t start = current_;
    const SourceLocation location{current_, line_, column_};
    advance();
    while (peek() != '\0' && peek() != '"') {
        if (peek() == '\\' && peekNext() != '\0') advance();
        advance();
    }
    if (peek() == '"') advance();
    return makeToken(TokenKind::StringLiteral, start, location);
}

Token Lexer::character() {
    const std::size_t start = current_;
    const SourceLocation location{current_, line_, column_};
    advance();
    if (peek() == '\\' && peekNext() != '\0') advance();
    if (peek() != '\0' && peek() != '\n') advance();
    if (peek() == '\'') advance();
    return makeToken(TokenKind::CharacterLiteral, start, location);
}

Token Lexer::next() {
    skipWhitespaceAndComments();
    const SourceLocation location{current_, line_, column_};
    const std::size_t start = current_;
    const char c = peek();

    if (c == '\0') return makeToken(TokenKind::EndOfFile, start, location);
    if (isIdentifierStart(c)) return identifierOrKeyword();
    if (std::isdigit(static_cast<unsigned char>(c))) return number();
    if (c == '"') return string();
    if (c == '\'') return character();

    advance();
    switch (c) {
        case '+': return makeToken(match('=') ? TokenKind::PlusEqual : TokenKind::Plus, start, location);
        case '-':
            if (match('>')) return makeToken(TokenKind::Arrow, start, location);
            return makeToken(match('=') ? TokenKind::MinusEqual : TokenKind::Minus, start, location);
        case '*': return makeToken(match('=') ? TokenKind::StarEqual : TokenKind::Star, start, location);
        case '/': return makeToken(match('=') ? TokenKind::SlashEqual : TokenKind::Slash, start, location);
        case '%': return makeToken(match('=') ? TokenKind::PercentEqual : TokenKind::Percent, start, location);
        case '=': return makeToken(match('=') ? TokenKind::EqualEqual : TokenKind::Equal, start, location);
        case '!': return makeToken(match('=') ? TokenKind::NotEqual : TokenKind::Bang, start, location);
        case '>': return makeToken(match('=') ? TokenKind::GreaterEqual : TokenKind::Greater, start, location);
        case '<': return makeToken(match('=') ? TokenKind::LessEqual : TokenKind::Less, start, location);
        case '&': return makeToken(TokenKind::Ampersand, start, location);
        case '|': return makeToken(TokenKind::Pipe, start, location);
        case '^': return makeToken(TokenKind::Caret, start, location);
        case '~': return makeToken(TokenKind::Tilde, start, location);
        case '.': return makeToken(TokenKind::Dot, start, location);
        case ',': return makeToken(TokenKind::Comma, start, location);
        case ':': return makeToken(TokenKind::Colon, start, location);
        case ';': return makeToken(TokenKind::Semicolon, start, location);
        case '(': return makeToken(TokenKind::LeftParen, start, location);
        case ')': return makeToken(TokenKind::RightParen, start, location);
        case '{': return makeToken(TokenKind::LeftBrace, start, location);
        case '}': return makeToken(TokenKind::RightBrace, start, location);
        case '[': return makeToken(TokenKind::LeftBracket, start, location);
        case ']': return makeToken(TokenKind::RightBracket, start, location);
        default: return makeToken(TokenKind::Unknown, start, location);
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!atEnd()) tokens.push_back(next());
    tokens.push_back(next());
    return tokens;
}

bool Lexer::atEnd() const noexcept { return current_ >= source_.size(); }

const char *tokenKindName(TokenKind kind) noexcept {
    switch (kind) {
        case TokenKind::EndOfFile: return "eof";
        case TokenKind::Identifier: return "identifier";
        case TokenKind::IntegerLiteral: return "integer";
        case TokenKind::FloatLiteral: return "float";
        case TokenKind::StringLiteral: return "string";
        case TokenKind::CharacterLiteral: return "character";
        case TokenKind::Unknown: return "unknown";
        default: return "token";
    }
}

} // namespace hyper
