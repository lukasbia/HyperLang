#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Lexer/Token.h"

#include <cctype>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hyperlang::lexer {

namespace {

using KeywordMap = std::unordered_map<std::string_view, TokenKind>;

const KeywordMap &keywordTable() {
    static const KeywordMap table = {
        {"func", TokenKind::Func},
        {"else", TokenKind::Else},
        {"if", TokenKind::If},
        {"then", TokenKind::Then},
        {"endif", TokenKind::EndIf},
        {"while", TokenKind::While},
        {"true", TokenKind::True},
        {"false", TokenKind::False},
        {"do", TokenKind::Do},
        {"loop", TokenKind::Loop},
        {"endLoop", TokenKind::EndLoop},
        {"let", TokenKind::Let},
        {"var", TokenKind::Var},
        {"string", TokenKind::String},
        {"panic", TokenKind::Panic},
        {"input", TokenKind::Input},
        {"output", TokenKind::Output},
        {"init", TokenKind::Init},
        {"deinit", TokenKind::Deinit},
        {"int", TokenKind::Int},
        {"num", TokenKind::Num},
        {"enum", TokenKind::Enum},
        {"nil", TokenKind::Nil},
        {"class", TokenKind::Class},
        {"struct", TokenKind::Struct},
        {"protocol", TokenKind::Protocol},
        {"extension", TokenKind::Extension},
        {"typealias", TokenKind::Typealias},
        {"guard", TokenKind::Guard},
        {"switch", TokenKind::Switch},
        {"case", TokenKind::Case},
        {"default", TokenKind::Default},
        {"for", TokenKind::For},
        {"in", TokenKind::In},
        {"break", TokenKind::Break},
        {"continue", TokenKind::Continue},
        {"return", TokenKind::Return},
        {"defer", TokenKind::Defer},
        {"throw", TokenKind::Throw},
        {"throws", TokenKind::Throws},
        {"catch", TokenKind::Catch},
        {"async", TokenKind::Async},
        {"await", TokenKind::Await},
        {"some", TokenKind::Some},
        {"any", TokenKind::Any},
        {"self", TokenKind::Self},
        {"where", TokenKind::Where},
        {"get", TokenKind::Get},
        {"set", TokenKind::Set},
        {"mutating", TokenKind::Mutating},
        {"static", TokenKind::Static},
        {"final", TokenKind::Final},
        {"private", TokenKind::Private},
        {"public", TokenKind::Public},
        {"internal", TokenKind::Internal},
        {"operator", TokenKind::Operator},
        {"subscript", TokenKind::Subscript},
        {"associatedtype", TokenKind::AssociatedType},
        {"required", TokenKind::Required},
        {"convenience", TokenKind::Convenience},
        {"override", TokenKind::Override},
        {"weak", TokenKind::Weak},
        {"unowned", TokenKind::Unowned},
        {"borrow", TokenKind::Borrow},
        {"consume", TokenKind::Consume},
        {"yield", TokenKind::Yield},
        {"macro", TokenKind::Macro},
        {"attribute", TokenKind::Attribute},
        {"module", TokenKind::Module},
        {"package", TokenKind::Package},
        {"namespace", TokenKind::Namespace},
        {"source", TokenKind::Source},
        {"file", TokenKind::File},
        {"function", TokenKind::Function},
        {"property", TokenKind::Property},
        {"event", TokenKind::Event},
        {"signal", TokenKind::Signal},
        {"asynclet", TokenKind::AsyncLet},
        {"actor", TokenKind::Actor},
        {"task", TokenKind::Task},
        {"detach", TokenKind::Detach},
        {"isolated", TokenKind::Isolated},
        {"nonisolated", TokenKind::Nonisolated},
        {"sendable", TokenKind::Sendable},
        {"move", TokenKind::Move},
        {"copy", TokenKind::Copy},
        {"weakref", TokenKind::WeakRef},
        {"strongref", TokenKind::StrongRef},
        {"own", TokenKind::Own},
        {"shared", TokenKind::Shared},
        {"observe", TokenKind::Observe},
        {"synchronize", TokenKind::Synchronize},
        {"compile", TokenKind::Compile},
        {"extern", TokenKind::Extern}
    };

    return table;
}

bool isIdentifierStart(char character) {
    const auto value = static_cast<unsigned char>(character);
    return std::isalpha(value) != 0 || character == '_';
}

bool isIdentifierContinuation(char character) {
    const auto value = static_cast<unsigned char>(character);
    return std::isalnum(value) != 0 || character == '_';
}

bool isDecimalDigit(char character) {
    return std::isdigit(static_cast<unsigned char>(character)) != 0;
}

} // namespace

Lexer::Lexer(std::string_view source)
    : source_(source) {
}

char Lexer::peek(std::size_t offset) const {
    const std::size_t position = index_ + offset;

    if (position >= source_.size()) {
        return '\0';
    }

    return source_[position];
}

char Lexer::advance() {
    const char character = peek();

    if (character == '\0') {
        return character;
    }

    ++index_;

    if (character == '\n') {
        ++location_.line;
        location_.column = 1;
    } else {
        ++location_.column;
    }

    return character;
}

void Lexer::skipWhitespace() {
    bool consumed = true;

    while (consumed) {
        consumed = false;

        while (std::isspace(static_cast<unsigned char>(peek())) != 0) {
            advance();
            consumed = true;
        }

        if (peek() == '/' && peek(1) == '/') {
            while (peek() != '\0' && peek() != '\n') {
                advance();
            }

            consumed = true;
        }
    }
}

Token Lexer::identifier() {
    const SourceLocation start = location_;
    const std::size_t begin = index_;

    advance();

    while (isIdentifierContinuation(peek())) {
        advance();
    }

    const std::string spelling(source_.substr(begin, index_ - begin));
    const auto iterator = keywordTable().find(spelling);

    if (iterator != keywordTable().end()) {
        return {iterator->second, spelling, start};
    }

    return {TokenKind::Identifier, spelling, start};
}

Token Lexer::number() {
    const SourceLocation start = location_;
    const std::size_t begin = index_;
    bool hasDecimalPoint = false;

    while (isDecimalDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDecimalDigit(peek(1))) {
        hasDecimalPoint = true;
        advance();

        while (isDecimalDigit(peek())) {
            advance();
        }
    }

    const std::string spelling(source_.substr(begin, index_ - begin));

    if (hasDecimalPoint) {
        return {TokenKind::NumberLiteral, spelling, start};
    }

    return {TokenKind::IntegerLiteral, spelling, start};
}

Token Lexer::string() {
    const SourceLocation start = location_;
    std::string value;

    advance();

    while (peek() != '\0' && peek() != '"') {
        if (peek() == '\\' && peek(1) != '\0') {
            advance();
            value.push_back(advance());
            continue;
        }

        value.push_back(advance());
    }

    if (peek() == '"') {
        advance();
    }

    return {TokenKind::StringLiteral, std::move(value), start};
}

Token Lexer::directive() {
    const SourceLocation start = location_;

    advance();

    const std::size_t begin = index_;

    while (std::isalpha(static_cast<unsigned char>(peek())) != 0) {
        advance();
    }

    const std::string spelling(source_.substr(begin, index_ - begin));

    if (spelling == "include") {
        return {TokenKind::AtInclude, "@include", start};
    }

    if (spelling == "import") {
        return {TokenKind::AtImport, "@import", start};
    }

    return {TokenKind::AtDirective, "@" + spelling, start};
}

Token Lexer::symbol() {
    const SourceLocation start = location_;
    const char character = advance();

    switch (character) {
    case '{':
        return {TokenKind::LBrace, "{", start};
    case '}':
        return {TokenKind::RBrace, "}", start};
    case '(':
        return {TokenKind::LParen, "(", start};
    case ')':
        return {TokenKind::RParen, ")", start};
    case '[':
        return {TokenKind::LBracket, "[", start};
    case ']':
        return {TokenKind::RBracket, "]", start};
    case '.':
        return {TokenKind::Dot, ".", start};
    case ',':
        return {TokenKind::Comma, ",", start};
    case ':':
        return {TokenKind::Colon, ":", start};
    case ';':
        return {TokenKind::Semicolon, ";", start};
    case '+':
        return {TokenKind::Plus, "+", start};
    case '-':
        if (peek() == '>') {
            advance();
            return {TokenKind::Arrow, "->", start};
        }
        return {TokenKind::Minus, "-", start};
    case '*':
        return {TokenKind::Star, "*", start};
    case '/':
        return {TokenKind::Slash, "/", start};
    case '%':
        return {TokenKind::Percent, "%", start};
    case '!':
        if (peek() == '=') {
            advance();
            return {TokenKind::NotEqual, "!=", start};
        }
        return {TokenKind::Bang, "!", start};
    case '=':
        if (peek() == '=') {
            advance();
            return {TokenKind::EqualEqual, "==", start};
        }
        return {TokenKind::Equal, "=", start};
    case '<':
        if (peek() == '=') {
            advance();
            return {TokenKind::LessEqual, "<=", start};
        }
        return {TokenKind::Less, "<", start};
    case '>':
        if (peek() == '=') {
            advance();
            return {TokenKind::GreaterEqual, ">=", start};
        }
        return {TokenKind::Greater, ">", start};
    case '&':
        if (peek() == '&') {
            advance();
            return {TokenKind::AndAnd, "&&", start};
        }
        break;
    case '|':
        if (peek() == '|') {
            advance();
            return {TokenKind::OrOr, "||", start};
        }
        break;
    default:
        break;
    }

    return {TokenKind::Unknown, std::string(1, character), start};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespace();

        const char character = peek();

        if (character == '\0') {
            tokens.push_back({TokenKind::EndOfFile, "", location_});
            break;
        }

        if (character == '@') {
            tokens.push_back(directive());
            continue;
        }

        if (isIdentifierStart(character)) {
            tokens.push_back(identifier());
            continue;
        }

        if (isDecimalDigit(character)) {
            tokens.push_back(number());
            continue;
        }

        if (character == '"') {
            tokens.push_back(string());
            continue;
        }

        tokens.push_back(symbol());
    }

    return tokens;
}

} // namespace hyperlang::lexer
