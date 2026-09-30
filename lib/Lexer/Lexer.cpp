#include "hyperlang/Lexer/Lexer.h"

#include <array>
#include <cctype>
#include <string>
#include <string_view>

namespace hyperlang::lexer {

namespace {

using KeywordEntry = std::pair<std::string_view, TokenKind>;

// The language table is deliberately compiled into the lexer. Keywords are not loaded dynamically.
constexpr std::array<KeywordEntry, 128> keywordTable = {{
    {"func", TokenKind::Func}, {"else", TokenKind::Else}, {"if", TokenKind::If}, {"then", TokenKind::Then},
    {"endif", TokenKind::EndIf}, {"while", TokenKind::While}, {"true", TokenKind::True}, {"false", TokenKind::False},
    {"do", TokenKind::Do}, {"loop", TokenKind::Loop}, {"endLoop", TokenKind::EndLoop}, {"let", TokenKind::Let},
    {"var", TokenKind::Var}, {"string", TokenKind::String}, {"panic", TokenKind::Panic}, {"input", TokenKind::Input},
    {"output", TokenKind::Output}, {"init", TokenKind::Init}, {"deinit", TokenKind::Deinit}, {"int", TokenKind::Int},
    {"num", TokenKind::Num}, {"enum", TokenKind::Enum}, {"nil", TokenKind::Nil}, {"return", TokenKind::Return},
    {"guard", TokenKind::Guard}, {"switch", TokenKind::Switch}, {"case", TokenKind::Case}, {"default", TokenKind::Default},
    {"for", TokenKind::For}, {"in", TokenKind::In}, {"break", TokenKind::Break}, {"continue", TokenKind::Continue},
    {"defer", TokenKind::Defer}, {"throw", TokenKind::Throw}, {"throws", TokenKind::Throws}, {"catch", TokenKind::Catch},
    {"async", TokenKind::Async}, {"await", TokenKind::Await}, {"some", TokenKind::Some}, {"any", TokenKind::Any},
    {"self", TokenKind::Self}, {"where", TokenKind::Where}, {"get", TokenKind::Get}, {"set", TokenKind::Set},
    {"mutating", TokenKind::Mutating}, {"static", TokenKind::Static}, {"final", TokenKind::Final}, {"private", TokenKind::Private},
    {"public", TokenKind::Public}, {"internal", TokenKind::Internal}, {"operator", TokenKind::Operator},
    {"subscript", TokenKind::Subscript}, {"associatedtype", TokenKind::AssociatedType}, {"required", TokenKind::Required},
    {"convenience", TokenKind::Convenience}, {"override", TokenKind::Override}, {"weak", TokenKind::Weak},
    {"unowned", TokenKind::Unowned}, {"borrow", TokenKind::Borrow}, {"consume", TokenKind::Consume}, {"yield", TokenKind::Yield},
    {"macro", TokenKind::Macro}, {"attribute", TokenKind::Attribute}, {"module", TokenKind::Module}, {"package", TokenKind::Package},
    {"namespace", TokenKind::Namespace}, {"source", TokenKind::Source}, {"file", TokenKind::File}, {"function", TokenKind::Function},
    {"property", TokenKind::Property}, {"event", TokenKind::Event}, {"signal", TokenKind::Signal}, {"asynclet", TokenKind::AsyncLet},
    {"actor", TokenKind::Actor}, {"task", TokenKind::Task}, {"detach", TokenKind::Detach}, {"isolated", TokenKind::Isolated},
    {"nonisolated", TokenKind::Nonisolated}, {"sendable", TokenKind::Sendable}, {"move", TokenKind::Move}, {"copy", TokenKind::Copy},
    {"weakref", TokenKind::WeakRef}, {"strongref", TokenKind::StrongRef}, {"own", TokenKind::Own}, {"shared", TokenKind::Shared},
    {"observe", TokenKind::Observe}, {"synchronize", TokenKind::Synchronize}, {"compile", TokenKind::Compile}, {"extern", TokenKind::Extern},
    {"and", TokenKind::And}, {"or", TokenKind::Or}, {"not", TokenKind::Not}, {"struct", TokenKind::Module},
    {"protocol", TokenKind::Module}, {"extension", TokenKind::Module}, {"typealias", TokenKind::Module}, {"class", TokenKind::Module},
    {"whereis", TokenKind::Where}, {"inputValue", TokenKind::Input}, {"outputValue", TokenKind::Output}, {"create", TokenKind::Init},
    {"destroy", TokenKind::Deinit}, {"integer", TokenKind::Int}, {"number", TokenKind::Num}, {"forever", TokenKind::Loop},
    {"endloop", TokenKind::EndLoop}, {"suspend", TokenKind::Await}, {"resume", TokenKind::Await}, {"external", TokenKind::Extern},
    {"synchronized", TokenKind::Synchronize}, {"ownership", TokenKind::Own}, {"reference", TokenKind::StrongRef},
    {"weakReference", TokenKind::WeakRef}, {"sharedReference", TokenKind::Shared}
}};

TokenKind lookupKeyword(std::string_view spelling)
{
    for (const auto& entry : keywordTable) {
        if (entry.first == spelling) {
            return entry.second;
        }
    }

    return TokenKind::Identifier;
}

TokenKind lookupDirective(std::string_view spelling)
{
    if (spelling == "include") return TokenKind::AtInclude;
    if (spelling == "import") return TokenKind::AtImport;
    if (spelling == "file") return TokenKind::AtFile;
    if (spelling == "api") return TokenKind::AtAPI;
    if (spelling == "webLink") return TokenKind::AtWebLink;
    if (spelling == "database") return TokenKind::AtDatabase;
    return TokenKind::AtDirective;
}

} // namespace

Lexer::Lexer(std::string_view source)
    : source_(source)
{
}

const std::vector<Token>& Lexer::tokens() const noexcept
{
    return tokens_;
}

std::size_t Lexer::offset() const noexcept
{
    return index_;
}

SourceLocation Lexer::location() const noexcept
{
    return location_;
}

char Lexer::peek(std::size_t distance) const noexcept
{
    const std::size_t position = index_ + distance;
    return position < source_.size() ? source_[position] : '\0';
}

char Lexer::advance() noexcept
{
    const char character = peek();

    if (character == '\0') return character;

    ++index_;

    if (character == '\n') {
        ++location_.line;
        location_.column = 1;
    } else {
        ++location_.column;
    }

    return character;
}

bool Lexer::atEnd() const noexcept
{
    return index_ >= source_.size();
}

bool Lexer::match(char expected) noexcept
{
    if (peek() != expected) return false;
    advance();
    return true;
}

bool Lexer::isIdentifierStart(char character) noexcept
{
    return std::isalpha(static_cast<unsigned char>(character)) || character == '_';
}

bool Lexer::isIdentifierContinue(char character) noexcept
{
    return std::isalnum(static_cast<unsigned char>(character)) || character == '_';
}

bool Lexer::isDecimalDigit(char character) noexcept
{
    return std::isdigit(static_cast<unsigned char>(character));
}

bool Lexer::isHexDigit(char character) noexcept
{
    return std::isdigit(static_cast<unsigned char>(character)) ||
           (character >= 'a' && character <= 'f') ||
           (character >= 'A' && character <= 'F');
}

int Lexer::hexadecimalValue(char character) noexcept
{
    if (character >= '0' && character <= '9') return character - '0';
    if (character >= 'a' && character <= 'f') return character - 'a' + 10;
    if (character >= 'A' && character <= 'F') return character - 'A' + 10;
    return -1;
}

Token Lexer::makeToken(TokenKind kind, SourceLocation start) const
{
    return makeToken(kind, start, start.offset);
}

Token Lexer::makeToken(TokenKind kind, SourceLocation start, std::size_t begin) const
{
    return Token{
        kind,
        std::string(source_.substr(begin, index_ - begin)),
        SourceRange{start, location_}
    };
}

void Lexer::skipWhitespace()
{
    for (;;) {
        while (std::isspace(static_cast<unsigned char>(peek()))) advance();

        if (peek() == '/' && peek(1) == '/') {
            skipLineComment();
            continue;
        }

        if (peek() == '/' && peek(1) == '*') {
            skipBlockComment();
            continue;
        }

        return;
    }
}

void Lexer::skipLineComment()
{
    while (!atEnd() && peek() != '\n') advance();
}

void Lexer::skipBlockComment()
{
    advance();
    advance();

    while (!atEnd()) {
        if (peek() == '*' && peek(1) == '/') {
            advance();
            advance();
            return;
        }
        advance();
    }
}

Token Lexer::lexIdentifierOrKeyword()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;

    while (isIdentifierContinue(peek())) advance();

    return makeToken(
        lookupKeyword(source_.substr(begin, index_ - begin)),
        start,
        begin);
}

Token Lexer::lexNumber()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    bool floatingPoint = false;

    if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X')) {
        advance();
        advance();
        while (isHexDigit(peek()) || peek() == '_') advance();
        return makeToken(TokenKind::IntegerLiteral, start, begin);
    }

    while (isDecimalDigit(peek()) || peek() == '_') advance();

    if (peek() == '.' && isDecimalDigit(peek(1))) {
        floatingPoint = true;
        advance();
        while (isDecimalDigit(peek()) || peek() == '_') advance();
    }

    if (peek() == 'e' || peek() == 'E') {
        floatingPoint = true;
        advance();
        if (peek() == '+' || peek() == '-') advance();
        while (isDecimalDigit(peek()) || peek() == '_') advance();
    }

    return makeToken(
        floatingPoint ? TokenKind::NumberLiteral : TokenKind::IntegerLiteral,
        start,
        begin);
}

Token Lexer::lexString()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    advance();

    while (!atEnd()) {
        if (peek() == '\\') {
            advance();
            if (!atEnd()) advance();
            continue;
        }

        if (peek() == '"') {
            advance();
            break;
        }

        advance();
    }

    return makeToken(TokenKind::StringLiteral, start, begin);
}

Token Lexer::lexCharacter()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    advance();

    if (peek() == '\\') {
        advance();
        if (!atEnd()) advance();
    } else if (!atEnd()) {
        advance();
    }

    if (peek() == '\'') advance();
    return makeToken(TokenKind::CharacterLiteral, start, begin);
}

Token Lexer::lexDirective()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    advance();

    while (isIdentifierContinue(peek())) advance();

    const std::string_view spelling = source_.substr(
        begin + 1,
        index_ - begin - 1);

    return makeToken(lookupDirective(spelling), start, begin);
}

Token Lexer::lexHashDirective()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    advance();

    while (isIdentifierContinue(peek())) advance();
    return makeToken(TokenKind::HashDirective, start, begin);
}

Token Lexer::lexOperatorOrPunctuation()
{
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    const char character = advance();

    switch (character) {
    case '{': return makeToken(TokenKind::LBrace, start, begin);
    case '}': return makeToken(TokenKind::RBrace, start, begin);
    case '(': return makeToken(TokenKind::LParen, start, begin);
    case ')': return makeToken(TokenKind::RParen, start, begin);
    case '[': return makeToken(TokenKind::LBracket, start, begin);
    case ']': return makeToken(TokenKind::RBracket, start, begin);
    case '.':
        if (peek() == '.' && peek(1) == '.') {
            advance();
            advance();
            return makeToken(TokenKind::Range, start, begin);
        }
        return makeToken(TokenKind::Dot, start, begin);
    case ',': return makeToken(TokenKind::Comma, start, begin);
    case ':': return makeToken(TokenKind::Colon, start, begin);
    case ';': return makeToken(TokenKind::Semicolon, start, begin);
    case '?': return makeToken(TokenKind::Question, start, begin);
    case '+':
        return match('=') ? makeToken(TokenKind::PlusEqual, start, begin) : makeToken(TokenKind::Plus, start, begin);
    case '-':
        if (match('>')) return makeToken(TokenKind::Arrow, start, begin);
        return match('=') ? makeToken(TokenKind::MinusEqual, start, begin) : makeToken(TokenKind::Minus, start, begin);
    case '*':
        return match('=') ? makeToken(TokenKind::StarEqual, start, begin) : makeToken(TokenKind::Star, start, begin);
    case '/':
        return match('=') ? makeToken(TokenKind::SlashEqual, start, begin) : makeToken(TokenKind::Slash, start, begin);
    case '%':
        return match('=') ? makeToken(TokenKind::PercentEqual, start, begin) : makeToken(TokenKind::Percent, start, begin);
    case '!':
        return match('=') ? makeToken(TokenKind::NotEqual, start, begin) : makeToken(TokenKind::Not, start, begin);
    case '=':
        return match('=') ? makeToken(TokenKind::EqualEqual, start, begin) : makeToken(TokenKind::Equal, start, begin);
    case '<':
        return match('=') ? makeToken(TokenKind::LessEqual, start, begin) : makeToken(TokenKind::Less, start, begin);
    case '>':
        return match('=') ? makeToken(TokenKind::GreaterEqual, start, begin) : makeToken(TokenKind::Greater, start, begin);
    case '&': return makeToken(TokenKind::BitAnd, start, begin);
    case '|': return makeToken(TokenKind::BitOr, start, begin);
    case '^': return makeToken(TokenKind::Caret, start, begin);
    case '~': return makeToken(TokenKind::Tilde, start, begin);
    default: return makeToken(TokenKind::Unknown, start, begin);
    }
}

std::vector<Token> Lexer::tokenize()
{
    tokens_.clear();
    index_ = 0;
    location_ = SourceLocation{};

    while (!atEnd()) {
        skipWhitespace();
        if (atEnd()) break;

        const char character = peek();

        if (isIdentifierStart(character)) {
            tokens_.push_back(lexIdentifierOrKeyword());
        } else if (isDecimalDigit(character)) {
            tokens_.push_back(lexNumber());
        } else if (character == '"') {
            tokens_.push_back(lexString());
        } else if (character == '\'') {
            tokens_.push_back(lexCharacter());
        } else if (character == '@') {
            tokens_.push_back(lexDirective());
        } else if (character == '#') {
            tokens_.push_back(lexHashDirective());
        } else {
            tokens_.push_back(lexOperatorOrPunctuation());
        }
    }

    tokens_.push_back({
        TokenKind::EndOfFile,
        {},
        SourceRange{location_, location_}
    });

    return tokens_;
}

} // namespace hyperlang::lexer
