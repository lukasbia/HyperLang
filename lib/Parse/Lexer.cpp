#include "hyper/Lexer.h"
#include "hyper/LexerOptions.h"
#include "hyper/LexerCharacter.h"
#include "hyper/LexerToken.h"
#include "hyper/LexerSupport.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hyper {

namespace {

struct UTF8Character {
    std::uint32_t value = 0;
    std::size_t width = 0;
    bool valid = false;
};

constexpr std::uint32_t InvalidScalar = 0xFFFD;

UTF8Character decodeUTF8(
    std::string_view source,
    std::size_t offset) {
    if (offset >= source.size()) {
        return {};
    }

    const unsigned char first =
        static_cast<unsigned char>(
            source[offset]);

    if (first < 0x80) {
        return {first, 1, true};
    }

    std::size_t width = 0;
    std::uint32_t value = 0;

    if ((first & 0xE0) == 0xC0) {
        width = 2;
        value = first & 0x1F;
    } else if ((first & 0xF0) == 0xE0) {
        width = 3;
        value = first & 0x0F;
    } else if ((first & 0xF8) == 0xF0) {
        width = 4;
        value = first & 0x07;
    } else {
        return {};
    }

    if (offset + width > source.size()) {
        return {};
    }

    for (std::size_t index = 1;
         index < width;
         ++index) {
        const unsigned char continuation =
            static_cast<unsigned char>(
                source[offset + index]);

        if ((continuation & 0xC0) != 0x80) {
            return {};
        }

        value = (value << 6) |
                (continuation & 0x3F);
    }

    if ((width == 2 && value < 0x80) ||
        (width == 3 && value < 0x800) ||
        (width == 4 && value < 0x10000)) {
        return {};
    }

    if (value > 0x10FFFF) {
        return {};
    }

    if (value >= 0xD800 &&
        value <= 0xDFFF) {
        return {};
    }

    return {value, width, true};
}

bool isValidScalar(
    std::uint32_t value) noexcept {
    return value <= 0x10FFFF &&
           !(value >= 0xD800 &&
             value <= 0xDFFF);
}

bool isIdentifierContinue(
    std::uint32_t value) noexcept {
    if (value < 0x80) {
        return std::isalnum(
                   static_cast<unsigned char>(
                       value)) != 0 ||
               value == '_';
    }

    return
        (value >= 0x00A8 && value <= 0x00FF) ||
        (value >= 0x0100 && value <= 0x02FF) ||
        (value >= 0x0370 && value <= 0x052F) ||
        (value >= 0x0530 && value <= 0x058F) ||
        (value >= 0x0590 && value <= 0x05FF) ||
        (value >= 0x0600 && value <= 0x06FF) ||
        (value >= 0x0700 && value <= 0x074F) ||
        (value >= 0x0780 && value <= 0x07BF) ||
        (value >= 0x0900 && value <= 0x0DFF) ||
        (value >= 0x0E00 && value <= 0x0FFF) ||
        (value >= 0x1000 && value <= 0x1FFF) ||
        (value >= 0x2000 && value <= 0x2FFF) ||
        (value >= 0x3000 && value <= 0x303F) ||
        (value >= 0x3040 && value <= 0x30FF) ||
        (value >= 0x3100 && value <= 0x31FF) ||
        (value >= 0x3400 && value <= 0x4DBF) ||
        (value >= 0x4E00 && value <= 0x9FFF) ||
        (value >= 0xA000 && value <= 0xA4CF) ||
        (value >= 0xAC00 && value <= 0xD7FF) ||
        (value >= 0xF900 && value <= 0xFAFF) ||
        (value >= 0xFE70 && value <= 0xFEFF) ||
        (value >= 0x10000 && value <= 0x1FFFD) ||
        (value >= 0x20000 && value <= 0x2FFFD) ||
        (value >= 0x30000 && value <= 0x3FFFD) ||
        (value >= 0x40000 && value <= 0x4FFFD) ||
        (value >= 0x50000 && value <= 0x5FFFD) ||
        (value >= 0x60000 && value <= 0x6FFFD) ||
        (value >= 0x70000 && value <= 0x7FFFD) ||
        (value >= 0x80000 && value <= 0x8FFFD) ||
        (value >= 0x90000 && value <= 0x9FFFD) ||
        (value >= 0xA0000 && value <= 0xAFFFD) ||
        (value >= 0xB0000 && value <= 0xBFFFD) ||
        (value >= 0xC0000 && value <= 0xCFFFD) ||
        (value >= 0xD0000 && value <= 0xDFFFD) ||
        (value >= 0xE0000 && value <= 0xEFFFD);
}

bool isIdentifierStart(
    std::uint32_t value) noexcept {
    if (value < 0x80) {
        return std::isalpha(
                   static_cast<unsigned char>(
                       value)) != 0 ||
               value == '_';
    }

    return isIdentifierContinue(value) &&
           !(value >= 0x0300 &&
             value <= 0x036F) &&
           !(value >= 0x1DC0 &&
             value <= 0x1DFF) &&
           !(value >= 0x20D0 &&
             value <= 0x20FF) &&
           !(value >= 0xFE20 &&
             value <= 0xFE2F);
}

bool isUnicodeWhitespace(
    std::uint32_t value) noexcept {
    switch (value) {
    case 0x0009:
    case 0x000A:
    case 0x000B:
    case 0x000C:
    case 0x000D:
    case 0x0020:
    case 0x0085:
    case 0x00A0:
    case 0x1680:
    case 0x2000:
    case 0x2001:
    case 0x2002:
    case 0x2003:
    case 0x2004:
    case 0x2005:
    case 0x2006:
    case 0x2007:
    case 0x2008:
    case 0x2009:
    case 0x200A:
    case 0x2028:
    case 0x2029:
    case 0x202F:
    case 0x205F:
    case 0x3000:
        return true;
    default:
        return false;
    }
}

bool isZeroWidth(
    std::uint32_t value) noexcept {
    return value == 0x200B ||
           value == 0x200C ||
           value == 0x200D ||
           value == 0x2060 ||
           value == 0xFEFF;
}

bool isASCIIIdentifierStart(
    char value) noexcept {
    return std::isalpha(
               static_cast<unsigned char>(
                   value)) != 0 ||
           value == '_';
}

bool isASCIIIdentifierContinue(
    char value) noexcept {
    return std::isalnum(
               static_cast<unsigned char>(
                   value)) != 0 ||
           value == '_';
}

bool isOperatorCharacter(
    char value) noexcept {
    switch (value) {
    case '=':
    case '-':
    case '+':
    case '*':
    case '/':
    case '%':
    case '&':
    case '|':
    case '^':
    case '~':
    case '!':
    case '?':
    case '<':
    case '>':
    case '.':
        return true;
    default:
        return false;
    }
}

bool isHorizontalWhitespace(
    char value) noexcept {
    return value == ' ' ||
           value == '\t' ||
           value == '\f' ||
           value == '\v';
}

bool isLeftOperatorBoundary(
    char value) noexcept {
    return isHorizontalWhitespace(value) ||
           value == '\r' ||
           value == '\n' ||
           value == '(' ||
           value == '[' ||
           value == '{' ||
           value == ',' ||
           value == ';' ||
           value == ':';
}

bool isRightOperatorBoundary(
    char value) noexcept {
    return isHorizontalWhitespace(value) ||
           value == '\r' ||
           value == '\n' ||
           value == ')' ||
           value == ']' ||
           value == '}' ||
           value == ',' ||
           value == ';';
}

std::size_t countHashes(
    std::string_view source,
    std::size_t offset) {
    std::size_t result = 0;

    while (offset + result < source.size() &&
           source[offset + result] == '#') {
        ++result;
    }

    return result;
}

const std::unordered_map<
    std::string_view,
    TokenKind> &keywordTable() {
    static const std::unordered_map<
        std::string_view,
        TokenKind> table = {
        {"if", TokenKind::KwIf},
        {"else", TokenKind::KwElse},
        {"while", TokenKind::KwWhile},
        {"do", TokenKind::KwDo},
        {"for", TokenKind::KwFor},
        {"in", TokenKind::KwIn},
        {"func", TokenKind::KwFunc},
        {"var", TokenKind::KwVar},
        {"let", TokenKind::KwLet},
        {"const", TokenKind::KwConst},
        {"return", TokenKind::KwReturn},
        {"struct", TokenKind::KwStruct},
        {"class", TokenKind::KwClass},
        {"enum", TokenKind::KwEnum},
        {"protocol", TokenKind::KwProtocol},
        {"extension", TokenKind::KwExtension},
        {"import", TokenKind::KwImport},
        {"module", TokenKind::KwModule},
        {"package", TokenKind::KwPackage},
        {"namespace", TokenKind::KwNamespace},
        {"public", TokenKind::KwPublic},
        {"private", TokenKind::KwPrivate},
        {"internal", TokenKind::KwInternal},
        {"protected", TokenKind::KwProtected},
        {"static", TokenKind::KwStatic},
        {"final", TokenKind::KwFinal},
        {"override", TokenKind::KwOverride},
        {"init", TokenKind::KwInit},
        {"deinit", TokenKind::KwDeinit},
        {"defer", TokenKind::KwDefer},
        {"break", TokenKind::KwBreak},
        {"continue", TokenKind::KwContinue},
        {"true", TokenKind::KwTrue},
        {"false", TokenKind::KwFalse},
        {"nil", TokenKind::KwNil},
        {"and", TokenKind::KwAnd},
        {"or", TokenKind::KwOr},
        {"not", TokenKind::KwNot},
        {"move", TokenKind::KwMove},
        {"borrow", TokenKind::KwBorrow},
        {"consume", TokenKind::KwConsume},
        {"copy", TokenKind::KwCopy},
        {"own", TokenKind::KwOwn},
        {"shared", TokenKind::KwShared},
        {"weak", TokenKind::KwWeak},
        {"unowned", TokenKind::KwUnowned},
        {"auto", TokenKind::KwAuto},
        {"type", TokenKind::KwType},
        {"typealias", TokenKind::KwTypealias},
        {"associatedtype", TokenKind::KwAssociatedType},
        {"int", TokenKind::KwInt},
        {"float", TokenKind::KwFloat},
        {"bool", TokenKind::KwBool},
        {"string", TokenKind::KwString},
        {"bytes", TokenKind::KwBytes},
        {"num", TokenKind::KwNum},
        {"guard", TokenKind::KwGuard},
        {"switch", TokenKind::KwSwitch},
        {"case", TokenKind::KwCase},
        {"default", TokenKind::KwDefault},
        {"throw", TokenKind::KwThrow},
        {"throws", TokenKind::KwThrows},
        {"catch", TokenKind::KwCatch},
        {"async", TokenKind::KwAsync},
        {"await", TokenKind::KwAwait},
        {"task", TokenKind::KwTask},
        {"actor", TokenKind::KwActor},
        {"some", TokenKind::KwSome},
        {"any", TokenKind::KwAny},
        {"self", TokenKind::KwSelf},
        {"where", TokenKind::KwWhere},
        {"get", TokenKind::KwGet},
        {"set", TokenKind::KwSet},
        {"mutating", TokenKind::KwMutating},
        {"operator", TokenKind::KwOperator},
        {"subscript", TokenKind::KwSubscript},
        {"yield", TokenKind::KwYield},
        {"macro", TokenKind::KwMacro},
        {"attribute", TokenKind::KwAttribute},
        {"observe", TokenKind::KwObserve},
        {"synchronize", TokenKind::KwSynchronize},
        {"compile", TokenKind::KwCompile},
        {"extern", TokenKind::KwExtern},
        {"output", TokenKind::KwOutput},
        {"input", TokenKind::KwInput},
        {"panic", TokenKind::KwPanic},
        {"loop", TokenKind::KwLoop},
        {"endLoop", TokenKind::KwEndLoop},
        {"then", TokenKind::KwThen},
        {"endif", TokenKind::KwEndIf},
        {"source", TokenKind::KwSource},
        {"file", TokenKind::KwFile},
        {"function", TokenKind::KwFunction},
        {"property", TokenKind::KwProperty},
        {"event", TokenKind::KwEvent},
        {"signal", TokenKind::KwSignal},
        {"detached", TokenKind::KwDetached},
        {"isolated", TokenKind::KwIsolated},
        {"nonisolated", TokenKind::KwNonisolated},
        {"sendable", TokenKind::KwSendable},
    };

    return table;
}

const std::unordered_map<
    std::string_view,
    TokenKind> &operatorTable() {
    static const std::unordered_map<
        std::string_view,
        TokenKind> table = {
        {"+", TokenKind::Plus},
        {"-", TokenKind::Minus},
        {"*", TokenKind::Star},
        {"/", TokenKind::Slash},
        {"%", TokenKind::Percent},
        {"&", TokenKind::Ampersand},
        {"|", TokenKind::Pipe},
        {"^", TokenKind::Caret},
        {"~", TokenKind::Tilde},
        {"!", TokenKind::Bang},
        {"?", TokenKind::Question},
        {"=", TokenKind::Equal},
        {"==", TokenKind::EqualEqual},
        {"!=", TokenKind::NotEqual},
        {">", TokenKind::Greater},
        {">=", TokenKind::GreaterEqual},
        {"<", TokenKind::Less},
        {"<=", TokenKind::LessEqual},
        {"+=", TokenKind::PlusEqual},
        {"-=", TokenKind::MinusEqual},
        {"*=", TokenKind::StarEqual},
        {"/=", TokenKind::SlashEqual},
        {"%=", TokenKind::PercentEqual},
        {"&=", TokenKind::AmpersandEqual},
        {"|=", TokenKind::PipeEqual},
        {"^=", TokenKind::CaretEqual},
        {"->", TokenKind::Arrow},
        {"=>", TokenKind::FatArrow},
        {"..", TokenKind::Range},
        {"...", TokenKind::ClosedRange},
        {"??", TokenKind::NilCoalescing},
        {"**", TokenKind::Power},
        {"<<", TokenKind::ShiftLeft},
        {">>", TokenKind::ShiftRight},
        {"<<=", TokenKind::ShiftLeftEqual},
        {">>=", TokenKind::ShiftRightEqual},
        {"&&", TokenKind::AndAnd},
        {"||", TokenKind::OrOr},
        {"~=", TokenKind::TildeEqual},
        {"::", TokenKind::DoubleColon},
    };

    return table;
}

} // namespace

Lexer::Lexer(std::string_view source)
    : source_(source) {
    if (source_.size() >= 3 &&
        static_cast<unsigned char>(
            source_[0]) == 0xEF &&
        static_cast<unsigned char>(
            source_[1]) == 0xBB &&
        static_cast<unsigned char>(
            source_[2]) == 0xBF) {
        current_ = 3;
    }
}

char Lexer::peekCharacter(
    std::size_t distance) const noexcept {
    const std::size_t position =
        current_ + distance;

    if (position >= source_.size()) {
        return '\0';
    }

    return source_[position];
}

char Lexer::advance() noexcept {
    if (current_ >= source_.size()) {
        return '\0';
    }

    const char value =
        source_[current_++];

    if (value == '\r') {
        if (current_ < source_.size() &&
            source_[current_] == '\n') {
            ++current_;
        }

        ++line_;
        column_ = 1;
        atStartOfLine_ = true;
        return value;
    }

    if (value == '\n') {
        ++line_;
        column_ = 1;
        atStartOfLine_ = true;
        return value;
    }

    ++column_;

    if (!isHorizontalWhitespace(value)) {
        atStartOfLine_ = false;
    }

    return value;
}

bool Lexer::match(
    char expected) noexcept {
    if (peekCharacter() != expected) {
        return false;
    }

    advance();
    return true;
}

bool Lexer::startsWith(
    std::string_view text) const noexcept {
    if (current_ > source_.size()) {
        return false;
    }

    return source_.substr(
        current_,
        text.size()) == text;
}

SourceLocation Lexer::locationFromOffset(
    std::size_t offset) const {
    SourceLocation result;

    result.offset =
        std::min(offset, source_.size());
    result.line = 1;
    result.column = 1;

    for (std::size_t index = 0;
         index < result.offset;
         ++index) {
        const char value =
            source_[index];

        if (value == '\r') {
            if (index + 1 < result.offset &&
                source_[index + 1] == '\n') {
                ++index;
            }

            ++result.line;
            result.column = 1;
            continue;
        }

        if (value == '\n') {
            ++result.line;
            result.column = 1;
            continue;
        }

        ++result.column;
    }

    return result;
}

Token Lexer::makeToken(
    TokenKind kind,
    std::size_t start,
    SourceLocation location,
    bool malformed) const {
    Token token;

    token.kind = kind;

    token.text =
        std::string(source_.substr(
            start,
            current_ - start));

    token.location = location;
    token.atStartOfLine =
        atStartOfLine_;
    token.malformed =
        malformed;
    token.hasLeadingComment =
        hadLeadingComment_;

    return token;
}

void Lexer::diagnose(
    LexerDiagnostic::Severity severity,
    SourceLocation location,
    std::string message,
    std::string replacement) {
    diagnostics_.push_back({
        severity,
        location,
        std::move(message),
        std::move(replacement)
    });
}

void Lexer::diagnoseError(
    SourceLocation location,
    std::string message,
    std::string replacement) {
    diagnose(
        LexerDiagnostic::Severity::Error,
        location,
        std::move(message),
        std::move(replacement));
}

void Lexer::diagnoseWarning(
    SourceLocation location,
    std::string message,
    std::string replacement) {
    diagnose(
        LexerDiagnostic::Severity::Warning,
        location,
        std::move(message),
        std::move(replacement));
}

void Lexer::reset() {
    current_ = 0;
    line_ = 1;
    column_ = 1;
    atStartOfLine_ = true;
    hadLeadingComment_ = false;
    leadingComment_.clear();
    lookaheadBuffer_.clear();

    if (source_.size() >= 3 &&
        static_cast<unsigned char>(
            source_[0]) == 0xEF &&
        static_cast<unsigned char>(
            source_[1]) == 0xBB &&
        static_cast<unsigned char>(
            source_[2]) == 0xBF) {
        current_ = 3;
    }
}

void Lexer::setKeepComments(
    bool keep) {
    if (keepComments_ == keep) {
        return;
    }

    keepComments_ = keep;
    invalidateLookahead();
}

void Lexer::setAllowHashbang(
    bool allow) {
    if (allowHashbang_ == allow) {
        return;
    }

    allowHashbang_ = allow;
    invalidateLookahead();
}

void Lexer::setAllowRegexLiterals(
    bool allow) {
    if (allowRegexLiterals_ == allow) {
        return;
    }

    allowRegexLiterals_ = allow;
    invalidateLookahead();
}

void Lexer::setTreatEditorPlaceholdersAsTokens(
    bool enabled) {
    if (treatEditorPlaceholdersAsTokens_ ==
        enabled) {
        return;
    }

    treatEditorPlaceholdersAsTokens_ =
        enabled;
    invalidateLookahead();
}

bool Lexer::keepComments()
    const noexcept {
    return keepComments_;
}

bool Lexer::allowHashbang()
    const noexcept {
    return allowHashbang_;
}

bool Lexer::allowRegexLiterals()
    const noexcept {
    return allowRegexLiterals_;
}

bool Lexer::atEnd()
    const noexcept {
    return current_ >= source_.size();
}

std::size_t Lexer::currentOffset()
    const noexcept {
    return current_;
}

SourceLocation Lexer::currentLocation()
    const noexcept {
    return {
        current_,
        line_,
        column_
    };
}

const std::vector<
    LexerDiagnostic> &
Lexer::diagnostics()
    const noexcept {
    return diagnostics_;
}

void Lexer::clearDiagnostics() {
    diagnostics_.clear();
}

bool Lexer::isWhitespaceAt(
    std::size_t offset) const noexcept {
    if (offset >= source_.size()) {
        return false;
    }

    const char value =
        source_[offset];

    if (std::isspace(
            static_cast<unsigned char>(
                value)) != 0) {
        return true;
    }

    const UTF8Character decoded =
        decodeUTF8(
            source_,
            offset);

    return decoded.valid &&
           isUnicodeWhitespace(
               decoded.value);
}

bool Lexer::isLineBreakAt(
    std::size_t offset) const noexcept {
    if (offset >= source_.size()) {
        return false;
    }

    return source_[offset] == '\r' ||
           source_[offset] == '\n';
}

bool Lexer::lexLineComment() {
    if (!startsWith("//")) {
        return false;
    }

    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const bool documentation =
        startsWith("///") ||
        startsWith("//!");

    advance();
    advance();

    while (!atEnd() &&
           !isLineBreakAt(current_)) {
        advance();
    }

    hadLeadingComment_ = true;

    leadingComment_.append(
        source_.substr(
            start,
            current_ - start));

    if (keepComments_) {
        lookaheadBuffer_.push_back(
            makeToken(
                documentation
                    ? TokenKind::DocComment
                    : TokenKind::Comment,
                start,
                location));
    }

    return true;
}

bool Lexer::lexBlockComment() {
    if (!startsWith("/*")) {
        return false;
    }

    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const bool documentation =
        startsWith("/**") ||
        startsWith("/*!");

    advance();
    advance();

    unsigned depth = 1;

    while (!atEnd() &&
           depth != 0) {
        if (startsWith("/*")) {
            advance();
            advance();
            ++depth;
            continue;
        }

        if (startsWith("*/")) {
            advance();
            advance();
            --depth;
            continue;
        }

        advance();
    }

    const bool malformed =
        depth != 0;

    if (malformed) {
        diagnoseError(
            location,
            "unterminated block comment",
            "*/");
    }

    hadLeadingComment_ = true;

    leadingComment_.append(
        source_.substr(
            start,
            current_ - start));

    if (keepComments_) {
        lookaheadBuffer_.push_back(
            makeToken(
                documentation
                    ? TokenKind::DocComment
                    : TokenKind::Comment,
                start,
                location,
                malformed));
    }

    return true;
}

bool Lexer::lexHashbang() {
    if (!startsWith("#!")) {
        return false;
    }

    if (current_ != 0 &&
        current_ != 3) {
        return false;
    }

    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    if (!allowHashbang_) {
        diagnoseError(
            location,
            "hashbang is not allowed here");
    }

    advance();
    advance();

    while (!atEnd() &&
           !isLineBreakAt(current_)) {
        advance();
    }

    hadLeadingComment_ = true;

    leadingComment_.append(
        source_.substr(
            start,
            current_ - start));

    if (keepComments_) {
        lookaheadBuffer_.push_back(
            makeToken(
                TokenKind::Hashbang,
                start,
                location));
    }

    return true;
}

bool Lexer::lexConflictMarker() {
    if (!atStartOfLine_) {
        return false;
    }

    if (!startsWith("<<<<<<< ") &&
        !startsWith(">>>>>>> ")) {
        return false;
    }

    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    diagnoseError(
        location,
        "unresolved source control conflict marker");

    while (!atEnd() &&
           !isLineBreakAt(current_)) {
        advance();
    }

    if (keepComments_) {
        lookaheadBuffer_.push_back(
            makeToken(
                TokenKind::ConflictMarker,
                start,
                location));
    }

    return true;
}

bool Lexer::lexCommentTrivia() {
    if (startsWith("//")) {
        return lexLineComment();
    }

    if (startsWith("/*")) {
        return lexBlockComment();
    }

    return false;
}

void Lexer::consumeTrivia() {
    hadLeadingComment_ = false;
    leadingComment_.clear();

    for (;;) {
        while (!atEnd() &&
               isWhitespaceAt(current_)) {
            advance();
        }

        if (atEnd()) {
            return;
        }

        if (lexCommentTrivia()) {
            if (keepComments_ &&
                !lookaheadBuffer_.empty()) {
                return;
            }

            continue;
        }

        if ((current_ == 0 ||
             current_ == 3) &&
            startsWith("#!")) {
            lexHashbang();

            if (keepComments_ &&
                !lookaheadBuffer_.empty()) {
                return;
            }

            continue;
        }

        if (atStartOfLine_ &&
            (startsWith("<<<<<<< ") ||
             startsWith(">>>>>>> "))) {
            lexConflictMarker();

            if (keepComments_ &&
                !lookaheadBuffer_.empty()) {
                return;
            }

            continue;
        }

        return;
    }
}

Token Lexer::lexIdentifierOrKeyword() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const UTF8Character first =
        decodeUTF8(
            source_,
            current_);

    if (!first.valid ||
        !isIdentifierStart(
            first.value)) {
        return lexUnknown();
    }

    current_ += first.width;
    column_ += first.width;

    while (!atEnd()) {
        const UTF8Character next =
            decodeUTF8(
                source_,
                current_);

        if (!next.valid ||
            !isIdentifierContinue(
                next.value)) {
            break;
        }

        current_ += next.width;
        column_ += next.width;
    }

    const std::string_view spelling =
        source_.substr(
            start,
            current_ - start);

    const auto keyword =
        keywordTable().find(spelling);

    if (keyword != keywordTable().end()) {
        return makeToken(
            keyword->second,
            start,
            location);
    }

    return makeToken(
        TokenKind::Identifier,
        start,
        location);
}

Token Lexer::lexEscapedIdentifier() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();

    const std::size_t bodyStart =
        current_;

    while (!atEnd() &&
           static_cast<unsigned char>(
               peekCharacter()) != 96) {
        if (isLineBreakAt(current_)) {
            diagnoseError(
                currentLocation(),
                "escaped identifier cannot contain a newline");
            break;
        }

        const UTF8Character decoded =
            decodeUTF8(
                source_,
                current_);

        if (!decoded.valid) {
            diagnoseError(
                currentLocation(),
                "invalid UTF-8 in escaped identifier");
            advance();
            continue;
        }

        if (isZeroWidth(
                decoded.value)) {
            diagnoseWarning(
                currentLocation(),
                "invisible character in escaped identifier");
        }

        current_ += decoded.width;
        column_ += decoded.width;
    }

    if (static_cast<unsigned char>(
            peekCharacter()) != 96) {
        diagnoseError(
            location,
            "unterminated escaped identifier",
            "identifier");
        return makeToken(
            TokenKind::Unknown,
            start,
            location,
            true);
    }

    const std::size_t bodyEnd =
        current_;

    advance();

    const std::string_view body =
        source_.substr(
            bodyStart,
            bodyEnd - bodyStart);

    if (body.empty() ||
        isEscapedIdentifierEntirelyWhitespace(
            body)) {
        diagnoseError(
            location,
            "escaped identifier must contain visible characters");
    }

    Token token =
        makeToken(
            TokenKind::EscapedIdentifier,
            start,
            location);

    token.escapedIdentifier = true;

    return token;
}

Token Lexer::lexDollarIdentifier() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();

    bool allDigits = true;

    while (!atEnd() &&
           isASCIIIdentifierContinue(
               peekCharacter())) {
        if (!isASCIIDigit(
                peekCharacter())) {
            allDigits = false;
        }

        advance();
    }

    return makeToken(
        allDigits
            ? TokenKind::DollarIdentifier
            : TokenKind::Identifier,
        start,
        location);
}

bool Lexer::scanDecimalDigits(
    std::size_t &position,
    bool allowUnderscores,
    std::size_t &count,
    bool &hadSeparators) {
    count = 0;
    hadSeparators = false;

    bool previousDigit = false;

    while (position < source_.size()) {
        const char value =
            source_[position];

        if (isASCIIDigit(value)) {
            ++position;
            ++count;
            previousDigit = true;
            continue;
        }

        if (allowUnderscores &&
            value == '_') {
            const bool nextDigit =
                position + 1 < source_.size() &&
                isASCIIDigit(
                    source_[position + 1]);

            if (!previousDigit ||
                !nextDigit) {
                diagnoseError(
                    locationFromOffset(position),
                    "invalid separator in numeric literal");
                break;
            }

            ++position;
            hadSeparators = true;
            previousDigit = false;
            continue;
        }

        break;
    }

    return count != 0;
}

bool Lexer::scanBasedDigits(
    std::size_t &position,
    NumberBase base,
    std::size_t &count,
    bool &hadSeparators) {
    count = 0;
    hadSeparators = false;

    const auto validDigit =
        [base](char value) {
            switch (base) {
            case NumberBase::Binary:
                return value == '0' ||
                       value == '1';
            case NumberBase::Octal:
                return value >= '0' &&
                       value <= '7';
            case NumberBase::Decimal:
                return std::isdigit(
                    static_cast<unsigned char>(
                        value)) != 0;
            case NumberBase::Hexadecimal:
                return std::isxdigit(
                    static_cast<unsigned char>(
                        value)) != 0;
            }

            return false;
        };

    bool previousDigit = false;

    while (position < source_.size()) {
        const char value =
            source_[position];

        if (validDigit(value)) {
            ++position;
            ++count;
            previousDigit = true;
            continue;
        }

        if (value == '_') {
            const bool nextDigit =
                position + 1 < source_.size() &&
                validDigit(
                    source_[position + 1]);

            if (!previousDigit ||
                !nextDigit) {
                diagnoseError(
                    locationFromOffset(position),
                    "invalid separator in numeric literal");
                break;
            }

            ++position;
            hadSeparators = true;
            previousDigit = false;
            continue;
        }

        break;
    }

    return count != 0;
}

Token Lexer::lexBinaryNumber() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();
    advance();

    std::size_t count = 0;
    bool separators = false;

    const bool valid =
        scanBasedDigits(
            current_,
            NumberBase::Binary,
            count,
            separators);

    if (!valid) {
        diagnoseError(
            currentLocation(),
            "binary integer literal requires digits");
    }

    Token token =
        makeToken(
            TokenKind::BinaryIntegerLiteral,
            start,
            location,
            !valid);

    token.numeric.base =
        NumberBase::Binary;
    token.numeric.hasSeparators =
        separators;

    return token;
}

Token Lexer::lexOctalNumber() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();
    advance();

    std::size_t count = 0;
    bool separators = false;

    const bool valid =
        scanBasedDigits(
            current_,
            NumberBase::Octal,
            count,
            separators);

    if (!valid) {
        diagnoseError(
            currentLocation(),
            "octal integer literal requires digits");
    }

    Token token =
        makeToken(
            TokenKind::OctalIntegerLiteral,
            start,
            location,
            !valid);

    token.numeric.base =
        NumberBase::Octal;
    token.numeric.hasSeparators =
        separators;

    return token;
}

Token Lexer::lexHexNumber() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();
    advance();

    std::size_t count = 0;
    bool separators = false;

    if (!scanBasedDigits(
            current_,
            NumberBase::Hexadecimal,
            count,
            separators)) {
        diagnoseError(
            currentLocation(),
            "hexadecimal integer literal requires digits");

        Token token =
            makeToken(
                TokenKind::HexIntegerLiteral,
                start,
                location,
                true);

        token.numeric.base =
            NumberBase::Hexadecimal;

        return token;
    }

    bool decimalPoint = false;
    bool exponent = false;
    bool malformed = false;

    if (peekCharacter() == '.' &&
        isASCIIHexDigit(
            peekCharacter(1))) {
        decimalPoint = true;
        advance();

        std::size_t fraction = 0;
        bool fractionSeparators = false;

        scanBasedDigits(
            current_,
            NumberBase::Hexadecimal,
            fraction,
            fractionSeparators);

        separators =
            separators ||
            fractionSeparators;
    }

    if (peekCharacter() == 'p' ||
        peekCharacter() == 'P') {
        exponent = true;
        advance();

        if (peekCharacter() == '+' ||
            peekCharacter() == '-') {
            advance();
        }

        std::size_t exponentDigits = 0;
        bool exponentSeparators = false;

        if (!scanDecimalDigits(
                current_,
                true,
                exponentDigits,
                exponentSeparators)) {
            diagnoseError(
                currentLocation(),
                "hexadecimal float requires exponent digits");
            malformed = true;
        }

        separators =
            separators ||
            exponentSeparators;
    } else if (decimalPoint) {
        diagnoseError(
            currentLocation(),
            "hexadecimal floating literal requires a p exponent");
        malformed = true;
    }

    Token token =
        makeToken(
            decimalPoint || exponent
                ? TokenKind::HexFloatLiteral
                : TokenKind::HexIntegerLiteral,
            start,
            location,
            malformed);

    token.numeric.base =
        NumberBase::Hexadecimal;
    token.numeric.hasDecimalPoint =
        decimalPoint;
    token.numeric.hasExponent =
        exponent;
    token.numeric.exponentIsBinary =
        exponent;
    token.numeric.hasSeparators =
        separators;

    return token;
}

Token Lexer::lexDecimalNumber() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    std::size_t count = 0;
    bool separators = false;

    scanDecimalDigits(
        current_,
        true,
        count,
        separators);

    bool decimalPoint = false;
    bool exponent = false;
    bool malformed = false;

    if (peekCharacter() == '.' &&
        peekCharacter(1) != '.' &&
        isASCIIDigit(
            peekCharacter(1))) {
        decimalPoint = true;
        advance();

        std::size_t fraction = 0;
        bool fractionSeparators = false;

        scanDecimalDigits(
            current_,
            true,
            fraction,
            fractionSeparators);

        separators =
            separators ||
            fractionSeparators;
    }

    if (peekCharacter() == 'e' ||
        peekCharacter() == 'E') {
        exponent = true;
        advance();

        if (peekCharacter() == '+' ||
            peekCharacter() == '-') {
            advance();
        }

        std::size_t exponentDigits = 0;
        bool exponentSeparators = false;

        if (!scanDecimalDigits(
                current_,
                true,
                exponentDigits,
                exponentSeparators)) {
            diagnoseError(
                currentLocation(),
                "floating literal requires exponent digits");
            malformed = true;
        }

        separators =
            separators ||
            exponentSeparators;
    }

    Token token =
        makeToken(
            decimalPoint || exponent
                ? TokenKind::FloatLiteral
                : TokenKind::IntegerLiteral,
            start,
            location,
            malformed);

    token.numeric.base =
        NumberBase::Decimal;
    token.numeric.hasDecimalPoint =
        decimalPoint;
    token.numeric.hasExponent =
        exponent;
    token.numeric.hasSeparators =
        separators;

    return token;
}

Token Lexer::lexNumber() {
    if (startsWith("0b") ||
        startsWith("0B")) {
        return lexBinaryNumber();
    }

    if (startsWith("0o") ||
        startsWith("0O")) {
        return lexOctalNumber();
    }

    if (startsWith("0x") ||
        startsWith("0X")) {
        return lexHexNumber();
    }

    if (peekCharacter() == '.') {
        const std::size_t start =
            current_;

        const SourceLocation location =
            currentLocation();

        advance();

        std::size_t count = 0;
        bool separators = false;

        scanDecimalDigits(
            current_,
            true,
            count,
            separators);

        Token token =
            makeToken(
                TokenKind::FloatLiteral,
                start,
                location,
                count == 0);

        token.numeric.base =
            NumberBase::Decimal;
        token.numeric.hasDecimalPoint =
            true;
        token.numeric.hasSeparators =
            separators;

        return token;
    }

    return lexDecimalNumber();
}

bool Lexer::scanUnicodeEscape(
    std::size_t &position,
    std::uint32_t &value) {
    if (position >= source_.size() ||
        source_[position] != '{') {
        diagnoseError(
            locationFromOffset(position),
            "Unicode escape must use the form \\u{...}");
        return false;
    }

    ++position;
    value = 0;

    unsigned digits = 0;

    while (position < source_.size() &&
           isASCIIHexDigit(
               source_[position])) {
        const char digit =
            source_[position++];

        value <<= 4;

        if (digit >= '0' &&
            digit <= '9') {
            value |= static_cast<unsigned>(
                digit - '0');
        } else if (digit >= 'a' &&
                   digit <= 'f') {
            value |= static_cast<unsigned>(
                digit - 'a' + 10);
        } else {
            value |= static_cast<unsigned>(
                digit - 'A' + 10);
        }

        ++digits;
    }

    if (position >= source_.size() ||
        source_[position] != '}') {
        diagnoseError(
            locationFromOffset(position),
            "Unicode escape is missing its closing brace");
        return false;
    }

    ++position;

    if (digits == 0 ||
        digits > 8 ||
        !isValidScalar(value)) {
        diagnoseError(
            locationFromOffset(position),
            "invalid Unicode scalar escape");
        return false;
    }

    return true;
}

bool Lexer::scanEscapeSequence(
    std::size_t &position,
    std::string &decoded,
    bool &valid) {
    valid = true;
    decoded.clear();

    if (position >= source_.size() ||
        source_[position] != '\\') {
        return false;
    }

    const std::size_t start =
        position;

    ++position;

    if (position >= source_.size()) {
        diagnoseError(
            locationFromOffset(start),
            "unterminated escape sequence");
        valid = false;
        return true;
    }

    const char escaped =
        source_[position++];

    switch (escaped) {
    case '0': decoded.push_back('\0'); return true;
    case 'n': decoded.push_back('\n'); return true;
    case 'r': decoded.push_back('\r'); return true;
    case 't': decoded.push_back('\t'); return true;
    case 'b': decoded.push_back('\b'); return true;
    case 'f': decoded.push_back('\f'); return true;
    case 'v': decoded.push_back('\v'); return true;
    case '\\': decoded.push_back('\\'); return true;
    case '\"': decoded.push_back('\"'); return true;
    case '\'': decoded.push_back('\''); return true;
    case '\n': return true;
    case '\r':
        if (position < source_.size() &&
            source_[position] == '\n') {
            ++position;
        }
        return true;
    case 'u': {
        std::uint32_t scalar = 0;

        if (!scanUnicodeEscape(
                position,
                scalar)) {
            valid = false;
            return true;
        }

        if (!encodeUTF8(
                scalar,
                decoded)) {
            valid = false;
        }

        return true;
    }
    default:
        diagnoseError(
            locationFromOffset(start),
            "unknown escape sequence");
        decoded.push_back(escaped);
        valid = false;
        return true;
    }
}

bool Lexer::scanStringDelimiter(
    std::size_t &delimiterLength,
    bool &multiline) {
    delimiterLength = 0;
    multiline = false;

    if (peekCharacter() != '\"') {
        return false;
    }

    if (peekCharacter(1) == '\"' &&
        peekCharacter(2) == '\"') {
        delimiterLength = 3;
        multiline = true;
        return true;
    }

    delimiterLength = 1;
    return true;
}

bool Lexer::scanInterpolatedExpression(
    std::size_t openingOffset,
    std::size_t &closingOffset) {
    if (openingOffset >= source_.size()) {
        return false;
    }

    std::size_t position =
        current_;

    unsigned parentheses = 1;
    unsigned braces = 0;
    unsigned brackets = 0;

    bool escaped = false;
    bool singleQuote = false;
    bool doubleQuote = false;

    while (position < source_.size()) {
        const char value =
            source_[position];

        if (escaped) {
            escaped = false;
            ++position;
            continue;
        }

        if (value == '\\') {
            escaped = true;
            ++position;
            continue;
        }

        if (doubleQuote) {
            if (value == '\"') {
                doubleQuote = false;
            }

            ++position;
            continue;
        }

        if (singleQuote) {
            if (value == '\'') {
                singleQuote = false;
            }

            ++position;
            continue;
        }

        switch (value) {
        case '\"':
            doubleQuote = true;
            ++position;
            break;

        case '\'':
            singleQuote = true;
            ++position;
            break;

        case '(':
            ++parentheses;
            ++position;
            break;

        case ')':
            if (braces == 0 &&
                brackets == 0) {
                --parentheses;

                if (parentheses == 0) {
                    closingOffset =
                        position;
                    return true;
                }
            }

            ++position;
            break;

        case '{':
            ++braces;
            ++position;
            break;

        case '}':
            if (braces != 0) {
                --braces;
            }

            ++position;
            break;

        case '[':
            ++brackets;
            ++position;
            break;

        case ']':
            if (brackets != 0) {
                --brackets;
            }

            ++position;
            break;

        default:
            ++position;
            break;
        }
    }

    return false;
}

Token Lexer::lexString() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    std::size_t delimiterLength = 0;
    bool multiline = false;

    scanStringDelimiter(
        delimiterLength,
        multiline);

    for (std::size_t index = 0;
         index < delimiterLength;
         ++index) {
        advance();
    }

    bool interpolation = false;
    bool escapes = false;
    bool malformed = false;
    bool terminated = false;

    while (!atEnd()) {
        if (multiline &&
            startsWith("\"\"\"")) {
            advance();
            advance();
            advance();

            terminated = true;
            break;
        }

        if (!multiline &&
            peekCharacter() == '\"') {
            advance();

            terminated = true;
            break;
        }

        if (!multiline &&
            isLineBreakAt(current_)) {
            diagnoseError(
                location,
                "string literal cannot cross a line boundary");
            malformed = true;
            break;
        }

        if (peekCharacter() == '\\') {
            if (peekCharacter(1) == '(') {
                interpolation = true;

                advance();
                advance();

                std::size_t closing = 0;

                if (!scanInterpolatedExpression(
                        current_ - 1,
                        closing)) {
                    diagnoseError(
                        currentLocation(),
                        "unterminated string interpolation");
                    malformed = true;
                    break;
                }

                current_ =
                    closing + 1;

                const SourceLocation after =
                    locationFromOffset(
                        current_);

                line_ = after.line;
                column_ = after.column;
                continue;
            }

            std::size_t position =
                current_;

            std::string decoded;
            bool valid = true;

            scanEscapeSequence(
                position,
                decoded,
                valid);

            current_ = position;

            const SourceLocation after =
                locationFromOffset(
                    current_);

            line_ = after.line;
            column_ = after.column;

            escapes = true;
            malformed =
                malformed || !valid;
            continue;
        }

        const UTF8Character decoded =
            decodeUTF8(
                source_,
                current_);

        if (!decoded.valid) {
            diagnoseError(
                currentLocation(),
                "invalid UTF-8 in string literal");
            malformed = true;
            advance();
            continue;
        }

        if (decoded.value < 0x20 &&
            decoded.value != '\t' &&
            decoded.value != '\n' &&
            decoded.value != '\r') {
            diagnoseError(
                currentLocation(),
                "control character in string literal");
            malformed = true;
        }

        current_ += decoded.width;
        column_ += decoded.width;
    }

    if (!terminated) {
        diagnoseError(
            location,
            "unterminated string literal",
            multiline
                ? "\"\"\""
                : "\"");
        malformed = true;
    }

    Token token =
        makeToken(
            multiline
                ? TokenKind::MultilineStringLiteral
                : TokenKind::StringLiteral,
            start,
            location,
            malformed);

    token.string.kind =
        multiline
            ? StringKind::Multiline
            : StringKind::Normal;

    token.string.hasInterpolation =
        interpolation;

    token.string.hasEscapes =
        escapes;

    return token;
}

Token Lexer::lexRawString(
    unsigned delimiterLength) {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    for (unsigned index = 0;
         index < delimiterLength;
         ++index) {
        advance();
    }

    if (peekCharacter() != '\"') {
        diagnoseError(
            location,
            "raw string delimiter is missing its quote");

        return makeToken(
            TokenKind::Hash,
            start,
            location,
            true);
    }

    const bool multiline =
        startsWith("\"\"\"");

    if (multiline) {
        advance();
        advance();
        advance();
    } else {
        advance();
    }

    bool interpolation = false;
    bool terminated = false;
    bool malformed = false;

    while (!atEnd()) {
        const std::size_t quoteLength =
            multiline ? 3 : 1;

        if (peekCharacter() == '\"' &&
            current_ + quoteLength <=
                source_.size() &&
            (!multiline ||
             startsWith("\"\"\""))) {
            const std::size_t hashStart =
                current_ + quoteLength;

            bool closing = true;

            for (unsigned index = 0;
                 index < delimiterLength;
                 ++index) {
                if (hashStart + index >=
                        source_.size() ||
                    source_[hashStart + index] != '#') {
                    closing = false;
                    break;
                }
            }

            if (closing) {
                current_ =
                    hashStart +
                    delimiterLength;

                const SourceLocation after =
                    locationFromOffset(
                        current_);

                line_ = after.line;
                column_ = after.column;
                terminated = true;
                break;
            }
        }

        if (peekCharacter() == '\\') {
            std::size_t position =
                current_ + 1;

            unsigned hashes = 0;

            while (hashes <
                       delimiterLength &&
                   position <
                       source_.size() &&
                   source_[position] == '#') {
                ++hashes;
                ++position;
            }

            if (hashes ==
                    delimiterLength &&
                position <
                    source_.size() &&
                source_[position] == '(') {
                interpolation = true;
                current_ =
                    position + 1;

                const SourceLocation moved =
                    locationFromOffset(
                        current_);

                line_ = moved.line;
                column_ = moved.column;

                std::size_t closing = 0;

                if (!scanInterpolatedExpression(
                        current_ - 1,
                        closing)) {
                    diagnoseError(
                        currentLocation(),
                        "unterminated raw string interpolation");
                    malformed = true;
                    break;
                }

                current_ =
                    closing + 1;

                const SourceLocation after =
                    locationFromOffset(
                        current_);

                line_ = after.line;
                column_ = after.column;
                continue;
            }
        }

        advance();
    }

    if (!terminated) {
        diagnoseError(
            location,
            "unterminated raw string literal");
        malformed = true;
    }

    Token token =
        makeToken(
            multiline
                ? TokenKind::MultilineStringLiteral
                : TokenKind::StringLiteral,
            start,
            location,
            malformed);

    token.string.kind =
        multiline
            ? StringKind::RawMultiline
            : StringKind::Raw;

    token.string.customDelimiterLength =
        delimiterLength;

    token.string.hasInterpolation =
        interpolation;

    return token;
}

Token Lexer::lexCharacter() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();

    unsigned scalarCount = 0;
    bool malformed = false;

    while (!atEnd() &&
           peekCharacter() != '\'') {
        if (peekCharacter() == '\\') {
            std::size_t position =
                current_;

            std::string decoded;
            bool valid = true;

            scanEscapeSequence(
                position,
                decoded,
                valid);

            current_ = position;

            const SourceLocation after =
                locationFromOffset(
                    current_);

            line_ = after.line;
            column_ = after.column;

            ++scalarCount;
            malformed =
                malformed || !valid;

            continue;
        }

        const UTF8Character decoded =
            decodeUTF8(
                source_,
                current_);

        if (!decoded.valid) {
            diagnoseError(
                currentLocation(),
                "invalid UTF-8 in character literal");
            malformed = true;
            advance();
            continue;
        }

        if (decoded.value == '\r' ||
            decoded.value == '\n') {
            diagnoseError(
                currentLocation(),
                "character literal cannot contain a newline");
            malformed = true;
            break;
        }

        current_ += decoded.width;
        column_ += decoded.width;
        ++scalarCount;
    }

    if (peekCharacter() == '\'') {
        advance();
    } else {
        diagnoseError(
            location,
            "unterminated character literal",
            "'");
        malformed = true;
    }

    if (scalarCount != 1) {
        diagnoseError(
            location,
            "character literal must contain exactly one character");
        malformed = true;
    }

    return makeToken(
        TokenKind::CharacterLiteral,
        start,
        location,
        malformed);
}

Token Lexer::lexDirective() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();

    if (!isASCIIIdentifierStart(
            peekCharacter())) {
        return makeToken(
            TokenKind::AtSign,
            start,
            location);
    }

    while (!atEnd() &&
           isASCIIIdentifierContinue(
               peekCharacter())) {
        advance();
    }

    return makeToken(
        TokenKind::Directive,
        start,
        location);
}

Token Lexer::lexHashConstruct() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const std::size_t hashes =
        countHashes(
            source_,
            current_);

    const std::size_t following =
        current_ + hashes;

    if (following < source_.size() &&
        source_[following] == '\"') {
        return lexRawString(
            static_cast<unsigned>(
                hashes));
    }

    if (following < source_.size() &&
        isASCIIIdentifierStart(
            source_[following])) {
        for (std::size_t index = 0;
             index < hashes;
             ++index) {
            advance();
        }

        while (!atEnd() &&
               isASCIIIdentifierContinue(
                   peekCharacter())) {
            advance();
        }

        return makeToken(
            TokenKind::Directive,
            start,
            location);
    }

    advance();

    return makeToken(
        TokenKind::Hash,
        start,
        location);
}

bool Lexer::looksLikeEditorPlaceholder(
    std::string_view text) noexcept {
    return text.size() >= 4 &&
           text[0] == '<' &&
           text[1] == '#' &&
           text[text.size() - 2] == '#' &&
           text.back() == '>';
}

bool Lexer::looksLikeDirective(
    std::string_view text) noexcept {
    return text.size() >= 2 &&
           text.front() == '@' &&
           isASCIIIdentifierStart(
               text[1]);
}

bool Lexer::looksLikeConflictMarker(
    std::string_view text) noexcept {
    return text.starts_with("<<<<<<<") ||
           text.starts_with(">>>>>>>");
}

Token Lexer::lexRegex() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    advance();

    bool escaped = false;
    bool inClass = false;
    bool terminated = false;

    while (!atEnd()) {
        const char value =
            peekCharacter();

        if (isLineBreakAt(current_)) {
            break;
        }

        if (escaped) {
            escaped = false;
            advance();
            continue;
        }

        if (value == '\\') {
            escaped = true;
            advance();
            continue;
        }

        if (value == '[') {
            inClass = true;
            advance();
            continue;
        }

        if (value == ']' &&
            inClass) {
            inClass = false;
            advance();
            continue;
        }

        if (value == '/' &&
            !inClass) {
            advance();

            while (!atEnd() &&
                   isASCIIIdentifierContinue(
                       peekCharacter())) {
                advance();
            }

            terminated = true;
            break;
        }

        advance();
    }

    if (!terminated) {
        diagnoseError(
            location,
            "unterminated regex literal",
            "/");
    }

    return makeToken(
        TokenKind::RegexLiteral,
        start,
        location,
        !terminated);
}

Token Lexer::lexOperator() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    while (!atEnd() &&
           isOperatorCharacter(
               peekCharacter())) {
        if (peekCharacter() == '.' &&
            current_ != start &&
            source_[start] != '.') {
            break;
        }

        if (peekCharacter() == '/' &&
            (peekCharacter(1) == '/' ||
             peekCharacter(1) == '*')) {
            break;
        }

        advance();
    }

    const std::string_view spelling =
        source_.substr(
            start,
            current_ - start);

    const auto found =
        operatorTable().find(
            spelling);

    Token token =
        found == operatorTable().end()
            ? makeToken(
                TokenKind::Unknown,
                start,
                location,
                true)
            : makeToken(
                found->second,
                start,
                location);

    if (found == operatorTable().end()) {
        diagnoseError(
            location,
            "unknown operator sequence");
    }

    token.binding =
        classifyOperatorBinding(
            source_,
            start,
            current_);

    return token;
}

OperatorBinding Lexer::classifyOperatorBinding(
    std::string_view source,
    std::size_t start,
    std::size_t end) noexcept {
    if (start > end ||
        end > source.size()) {
        return OperatorBinding::None;
    }

    const bool leftBound =
        start != 0 &&
        !isLeftOperatorBoundary(
            source[start - 1]);

    const bool rightBound =
        end < source.size() &&
        !isRightOperatorBoundary(
            source[end]);

    if (leftBound == rightBound) {
        return leftBound
            ? OperatorBinding::BinaryUnspaced
            : OperatorBinding::BinarySpaced;
    }

    return leftBound
        ? OperatorBinding::Postfix
        : OperatorBinding::Prefix;
}

Token Lexer::lexUnknown() {
    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const UTF8Character decoded =
        decodeUTF8(
            source_,
            current_);

    if (!decoded.valid) {
        diagnoseError(
            location,
            "invalid UTF-8 sequence");

        advance();

        return makeToken(
            TokenKind::Unknown,
            start,
            location,
            true);
    }

    if (decoded.value == 0x00A0) {
        diagnoseWarning(
            location,
            "non-breaking space used as source whitespace",
            " ");

        current_ += decoded.width;
        column_ += decoded.width;

        return makeToken(
            TokenKind::Unknown,
            start,
            location,
            true);
    }

    if (isZeroWidth(
            decoded.value)) {
        diagnoseError(
            location,
            "invisible Unicode character is not allowed here");

        current_ += decoded.width;
        column_ += decoded.width;

        return makeToken(
            TokenKind::Unknown,
            start,
            location,
            true);
    }

    current_ += decoded.width;
    column_ += decoded.width;

    diagnoseError(
        location,
        "invalid character in source");

    return makeToken(
        TokenKind::Unknown,
        start,
        location,
        true);
}

void Lexer::invalidateLookahead() {
    lookaheadBuffer_.clear();
}

Token Lexer::next() {
    if (!lookaheadBuffer_.empty()) {
        Token result =
            std::move(
                lookaheadBuffer_.front());

        lookaheadBuffer_.erase(
            lookaheadBuffer_.begin());

        return result;
    }

    consumeTrivia();

    if (!lookaheadBuffer_.empty()) {
        Token result =
            std::move(
                lookaheadBuffer_.front());

        lookaheadBuffer_.erase(
            lookaheadBuffer_.begin());

        return result;
    }

    if (atEnd()) {
        Token token;

        token.kind =
            TokenKind::EndOfFile;
        token.location =
            currentLocation();
        token.atStartOfLine =
            atStartOfLine_;

        return token;
    }

    const std::size_t start =
        current_;

    const SourceLocation location =
        currentLocation();

    const char value =
        peekCharacter();

    if (startsWith("<#") &&
        treatEditorPlaceholdersAsTokens_) {
        const std::size_t end =
            source_.find(
                "#>",
                current_ + 2);

        const std::size_t line =
            source_.find(
                '\n',
                current_);

        if (end !=
                std::string_view::npos &&
            (line ==
                 std::string_view::npos ||
             end < line)) {
            current_ = end + 2;

            const SourceLocation after =
                locationFromOffset(
                    current_);

            line_ = after.line;
            column_ = after.column;

            return makeToken(
                TokenKind::EditorPlaceholder,
                start,
                location);
        }
    }

    if (value == '@') {
        return lexDirective();
    }

    if (value == '#') {
        return lexHashConstruct();
    }

    if (value == '$') {
        return lexDollarIdentifier();
    }

    if (static_cast<unsigned char>(
            value) == 96) {
        return lexEscapedIdentifier();
    }

    if (value == '\"') {
        return lexString();
    }

    if (value == '\'') {
        return lexCharacter();
    }

    if (isASCIIDigit(value)) {
        return lexNumber();
    }

    if (value == '.' &&
        isASCIIDigit(
            peekCharacter(1))) {
        return lexNumber();
    }

    if (isASCIIIdentifierStart(value) ||
        (static_cast<unsigned char>(
             value) &
         0x80) != 0) {
        return lexIdentifierOrKeyword();
    }

    if (value == '/' &&
        allowRegexLiterals_ &&
        peekCharacter(1) != '/' &&
        peekCharacter(1) != '*') {
        return lexRegex();
    }

    switch (value) {
    case '(':
        advance();
        return makeToken(
            TokenKind::LeftParen,
            start,
            location);

    case ')':
        advance();
        return makeToken(
            TokenKind::RightParen,
            start,
            location);

    case '{':
        advance();
        return makeToken(
            TokenKind::LeftBrace,
            start,
            location);

    case '}':
        advance();
        return makeToken(
            TokenKind::RightBrace,
            start,
            location);

    case '[':
        advance();
        return makeToken(
            TokenKind::LeftBracket,
            start,
            location);

    case ']':
        advance();
        return makeToken(
            TokenKind::RightBracket,
            start,
            location);

    case ',':
        advance();
        return makeToken(
            TokenKind::Comma,
            start,
            location);

    case ';':
        advance();
        return makeToken(
            TokenKind::Semicolon,
            start,
            location);

    case ':':
        advance();

        if (peekCharacter() == ':') {
            advance();

            return makeToken(
                TokenKind::DoubleColon,
                start,
                location);
        }

        return makeToken(
            TokenKind::Colon,
            start,
            location);

    case '\\':
        advance();

        return makeToken(
            TokenKind::Backslash,
            start,
            location);

    default:
        break;
    }

    if (isOperatorCharacter(
            value)) {
        return lexOperator();
    }

    return lexUnknown();
}

Token Lexer::peek() {
    return lookahead(0);
}

Token Lexer::lookahead(
    std::size_t distance) {
    const std::size_t savedCurrent =
        current_;

    const std::size_t savedLine =
        line_;

    const std::size_t savedColumn =
        column_;

    const bool savedStart =
        atStartOfLine_;

    const bool savedLeading =
        hadLeadingComment_;

    const std::string savedComment =
        leadingComment_;

    const std::size_t diagnosticsBefore =
        diagnostics_.size();

    const std::vector<Token> savedBuffer =
        lookaheadBuffer_;

    lookaheadBuffer_.clear();

    Token result;

    for (std::size_t index = 0;
         index <= distance;
         ++index) {
        result = next();
    }

    current_ = savedCurrent;
    line_ = savedLine;
    column_ = savedColumn;
    atStartOfLine_ = savedStart;
    hadLeadingComment_ = savedLeading;
    leadingComment_ = savedComment;
    lookaheadBuffer_ = savedBuffer;

    if (diagnostics_.size() >
        diagnosticsBefore) {
        diagnostics_.resize(
            diagnosticsBefore);
    }

    return result;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    tokens.reserve(
        source_.size() / 2 + 1);

    for (;;) {
        Token token =
            next();

        tokens.push_back(
            token);

        if (token.kind ==
            TokenKind::EndOfFile) {
            break;
        }
    }

    return tokens;
}

bool Lexer::isIdentifier(
    std::string_view text) {
    if (text.empty()) {
        return false;
    }

    std::size_t position = 0;

    const UTF8Character first =
        decodeUTF8(
            text,
            position);

    if (!first.valid ||
        !isIdentifierStart(
            first.value)) {
        return false;
    }

    position += first.width;

    while (position < text.size()) {
        const UTF8Character decoded =
            decodeUTF8(
                text,
                position);

        if (!decoded.valid ||
            !isIdentifierContinue(
                decoded.value)) {
            return false;
        }

        position += decoded.width;
    }

    return true;
}

bool Lexer::isOperator(
    std::string_view text) {
    if (text.empty()) {
        return false;
    }

    for (char value : text) {
        if (!isOperatorCharacter(
                value)) {
            return false;
        }
    }

    return true;
}

bool Lexer::isValidEscapedIdentifier(
    std::string_view text) {
    if (text.size() < 2 ||
        text.front() !=
            static_cast<char>(96) ||
        text.back() !=
            static_cast<char>(96)) {
        return false;
    }

    const std::string_view body =
        text.substr(
            1,
            text.size() - 2);

    if (body.empty() ||
        isEscapedIdentifierEntirelyWhitespace(
            body)) {
        return false;
    }

    std::size_t position = 0;

    while (position < body.size()) {
        const UTF8Character decoded =
            decodeUTF8(
                body,
                position);

        if (!decoded.valid ||
            decoded.value == 96 ||
            decoded.value == '\\') {
            return false;
        }

        position += decoded.width;
    }

    return true;
}

bool Lexer::isEscapedIdentifierEntirelyWhitespace(
    std::string_view text) {
    if (text.empty()) {
        return true;
    }

    std::size_t position = 0;

    while (position < text.size()) {
        const UTF8Character decoded =
            decodeUTF8(
                text,
                position);

        if (!decoded.valid ||
            !isUnicodeWhitespace(
                decoded.value)) {
            return false;
        }

        position += decoded.width;
    }

    return true;
}

bool Lexer::isASCIIIdentifierStart(
    char value) noexcept {
    return isASCIIIdentifierStartImpl(
        value);
}

bool Lexer::isASCIIIdentifierContinue(
    char value) noexcept {
    return isASCIIIdentifierContinueImpl(
        value);
}

bool Lexer::isASCIIDigit(
    char value) noexcept {
    return std::isdigit(
        static_cast<unsigned char>(
            value)) != 0;
}

bool Lexer::isASCIIHexDigit(
    char value) noexcept {
    return std::isxdigit(
        static_cast<unsigned char>(
            value)) != 0;
}

bool Lexer::isASCIIOctalDigit(
    char value) noexcept {
    return value >= '0' &&
           value <= '7';
}

bool Lexer::isASCIIWhitespace(
    char value) noexcept {
    return std::isspace(
        static_cast<unsigned char>(
            value)) != 0;
}

bool Lexer::isPrintableASCII(
    char value) noexcept {
    return std::isprint(
        static_cast<unsigned char>(
            value)) != 0;
}

bool Lexer::isUnicodeIdentifierStart(
    std::uint32_t value) noexcept {
    return isIdentifierStart(value);
}

bool Lexer::isUnicodeIdentifierContinue(
    std::uint32_t value) noexcept {
    return isIdentifierContinue(
        value);
}

bool Lexer::isRawIdentifierWhitespace(
    std::uint32_t value) noexcept {
    return value == 0x20 ||
           value == 0x200E ||
           value == 0x200F;
}

bool Lexer::isForbiddenIdentifierCodePoint(
    std::uint32_t value) noexcept {
    return value < 0x20 ||
           value == 0x7F ||
           isZeroWidth(value);
}

bool Lexer::isOperatorCharacter(
    char value) noexcept {
    return isOperatorCharacterImpl(
        value);
}

bool Lexer::isOperatorStartCharacter(
    char value) noexcept {
    return isOperatorCharacterImpl(
        value);
}

bool Lexer::isOperatorContinuationCharacter(
    char value) noexcept {
    return isOperatorCharacterImpl(
        value);
}

bool Lexer::encodeUTF8(
    std::uint32_t codePoint,
    std::string &output) {
    output.clear();

    if (!isValidScalar(
            codePoint)) {
        return false;
    }

    if (codePoint < 0x80) {
        output.push_back(
            static_cast<char>(
                codePoint));
        return true;
    }

    if (codePoint < 0x800) {
        output.push_back(
            static_cast<char>(
                0xC0 |
                (codePoint >> 6)));

        output.push_back(
            static_cast<char>(
                0x80 |
                (codePoint & 0x3F)));

        return true;
    }

    if (codePoint < 0x10000) {
        output.push_back(
            static_cast<char>(
                0xE0 |
                (codePoint >> 12)));

        output.push_back(
            static_cast<char>(
                0x80 |
                ((codePoint >> 6) &
                 0x3F)));

        output.push_back(
            static_cast<char>(
                0x80 |
                (codePoint & 0x3F)));

        return true;
    }

    output.push_back(
        static_cast<char>(
            0xF0 |
            (codePoint >> 18)));

    output.push_back(
        static_cast<char>(
            0x80 |
            ((codePoint >> 12) &
             0x3F)));

    output.push_back(
        static_cast<char>(
            0x80 |
            ((codePoint >> 6) &
             0x3F)));

    output.push_back(
        static_cast<char>(
            0x80 |
            (codePoint & 0x3F)));

    return true;
}

std::uint32_t Lexer::validateUTF8Character(
    std::string_view bytes,
    std::size_t &offset) {
    const UTF8Character decoded =
        decodeUTF8(
            bytes,
            offset);

    if (!decoded.valid) {
        if (offset < bytes.size()) {
            ++offset;
        }

        return InvalidScalar;
    }

    offset += decoded.width;

    return decoded.value;
}

TokenKind Lexer::keywordKind(
    std::string_view text) noexcept {
    const auto found =
        keywordTable().find(text);

    if (found == keywordTable().end()) {
        return TokenKind::Identifier;
    }

    return found->second;
}

TokenKind Lexer::punctuationKind(
    std::string_view text) noexcept {
    if (text == "@") return TokenKind::AtSign;
    if (text == "#") return TokenKind::Hash;
    if (text == ".") return TokenKind::Dot;
    if (text == ",") return TokenKind::Comma;
    if (text == ":") return TokenKind::Colon;
    if (text == "::") return TokenKind::DoubleColon;
    if (text == ";") return TokenKind::Semicolon;
    if (text == "\\") return TokenKind::Backslash;
    if (text == "(") return TokenKind::LeftParen;
    if (text == ")") return TokenKind::RightParen;
    if (text == "{") return TokenKind::LeftBrace;
    if (text == "}") return TokenKind::RightBrace;
    if (text == "[") return TokenKind::LeftBracket;
    if (text == "]") return TokenKind::RightBracket;
    return TokenKind::Unknown;
}

TokenKind Lexer::operatorKind(
    std::string_view text) noexcept {
    const auto found =
        operatorTable().find(text);

    if (found == operatorTable().end()) {
        return TokenKind::Unknown;
    }

    return found->second;
}

const char *tokenKindName(
    TokenKind kind) noexcept {
    switch (kind) {
    case TokenKind::EndOfFile: return "eof";
    case TokenKind::Unknown: return "unknown";
    case TokenKind::Identifier: return "identifier";
    case TokenKind::EscapedIdentifier: return "escaped-identifier";
    case TokenKind::DollarIdentifier: return "dollar-identifier";
    case TokenKind::IntegerLiteral: return "integer";
    case TokenKind::BinaryIntegerLiteral: return "binary-integer";
    case TokenKind::OctalIntegerLiteral: return "octal-integer";
    case TokenKind::HexIntegerLiteral: return "hex-integer";
    case TokenKind::FloatLiteral: return "float";
    case TokenKind::HexFloatLiteral: return "hex-float";
    case TokenKind::StringLiteral: return "string";
    case TokenKind::MultilineStringLiteral: return "multiline-string";
    case TokenKind::CharacterLiteral: return "character";
    case TokenKind::RegexLiteral: return "regex";
    case TokenKind::EditorPlaceholder: return "editor-placeholder";
    case TokenKind::AtSign: return "@";
    case TokenKind::Hash: return "#";
    case TokenKind::Directive: return "directive";
    case TokenKind::Plus: return "+";
    case TokenKind::Minus: return "-";
    case TokenKind::Star: return "*";
    case TokenKind::Slash: return "/";
    case TokenKind::Percent: return "%";
    case TokenKind::Ampersand: return "&";
    case TokenKind::Pipe: return "|";
    case TokenKind::Caret: return "^";
    case TokenKind::Tilde: return "~";
    case TokenKind::Bang: return "!";
    case TokenKind::Question: return "?";
    case TokenKind::Equal: return "=";
    case TokenKind::EqualEqual: return "==";
    case TokenKind::NotEqual: return "!=";
    case TokenKind::Greater: return ">";
    case TokenKind::GreaterEqual: return ">=";
    case TokenKind::Less: return "<";
    case TokenKind::LessEqual: return "<=";
    case TokenKind::PlusEqual: return "+=";
    case TokenKind::MinusEqual: return "-=";
    case TokenKind::StarEqual: return "*=";
    case TokenKind::SlashEqual: return "/=";
    case TokenKind::PercentEqual: return "%=";
    case TokenKind::AmpersandEqual: return "&=";
    case TokenKind::PipeEqual: return "|=";
    case TokenKind::CaretEqual: return "^=";
    case TokenKind::Arrow: return "->";
    case TokenKind::FatArrow: return "=>";
    case TokenKind::Range: return "..";
    case TokenKind::ClosedRange: return "...";
    case TokenKind::NilCoalescing: return "??";
    case TokenKind::Power: return "**";
    case TokenKind::ShiftLeft: return "<<";
    case TokenKind::ShiftRight: return ">>";
    case TokenKind::ShiftLeftEqual: return "<<=";
    case TokenKind::ShiftRightEqual: return ">>=";
    case TokenKind::AndAnd: return "&&";
    case TokenKind::OrOr: return "||";
    case TokenKind::QuestionQuestion: return "??";
    case TokenKind::TildeEqual: return "~=";
    case TokenKind::Dot: return ".";
    case TokenKind::Comma: return ",";
    case TokenKind::Colon: return ":";
    case TokenKind::DoubleColon: return "::";
    case TokenKind::Semicolon: return ";";
    case TokenKind::Backslash: return "\";
    case TokenKind::LeftParen: return "(";
    case TokenKind::RightParen: return ")";
    case TokenKind::LeftBrace: return "{";
    case TokenKind::RightBrace: return "}";
    case TokenKind::LeftBracket: return "[";
    case TokenKind::RightBracket: return "]";
    case TokenKind::Comment: return "comment";
    case TokenKind::DocComment: return "doc-comment";
    case TokenKind::Hashbang: return "hashbang";
    case TokenKind::ConflictMarker: return "conflict-marker";
    case TokenKind::KwIf: return "if";
    case TokenKind::KwElse: return "else";
    case TokenKind::KwWhile: return "while";
    case TokenKind::KwDo: return "do";
    case TokenKind::KwFor: return "for";
    case TokenKind::KwIn: return "in";
    case TokenKind::KwFunc: return "func";
    case TokenKind::KwVar: return "var";
    case TokenKind::KwLet: return "let";
    case TokenKind::KwConst: return "const";
    case TokenKind::KwReturn: return "return";
    case TokenKind::KwStruct: return "struct";
    case TokenKind::KwClass: return "class";
    case TokenKind::KwEnum: return "enum";
    case TokenKind::KwProtocol: return "protocol";
    case TokenKind::KwExtension: return "extension";
    case TokenKind::KwImport: return "import";
    case TokenKind::KwModule: return "module";
    case TokenKind::KwPackage: return "package";
    case TokenKind::KwNamespace: return "namespace";
    case TokenKind::KwPublic: return "public";
    case TokenKind::KwPrivate: return "private";
    case TokenKind::KwInternal: return "internal";
    case TokenKind::KwProtected: return "protected";
    case TokenKind::KwStatic: return "static";
    case TokenKind::KwFinal: return "final";
    case TokenKind::KwOverride: return "override";
    case TokenKind::KwInit: return "init";
    case TokenKind::KwDeinit: return "deinit";
    case TokenKind::KwDefer: return "defer";
    case TokenKind::KwBreak: return "break";
    case TokenKind::KwContinue: return "continue";
    case TokenKind::KwTrue: return "true";
    case TokenKind::KwFalse: return "false";
    case TokenKind::KwNil: return "nil";
    case TokenKind::KwAnd: return "and";
    case TokenKind::KwOr: return "or";
    case TokenKind::KwNot: return "not";
    case TokenKind::KwMove: return "move";
    case TokenKind::KwBorrow: return "borrow";
    case TokenKind::KwConsume: return "consume";
    case TokenKind::KwCopy: return "copy";
    case TokenKind::KwOwn: return "own";
    case TokenKind::KwShared: return "shared";
    case TokenKind::KwWeak: return "weak";
    case TokenKind::KwUnowned: return "unowned";
    case TokenKind::KwAuto: return "auto";
    case TokenKind::KwType: return "type";
    case TokenKind::KwTypealias: return "typealias";
    case TokenKind::KwAssociatedType: return "associatedtype";
    case TokenKind::KwInt: return "int";
    case TokenKind::KwFloat: return "float";
    case TokenKind::KwBool: return "bool";
    case TokenKind::KwString: return "string";
    case TokenKind::KwBytes: return "bytes";
    case TokenKind::KwNum: return "num";
    case TokenKind::KwGuard: return "guard";
    case TokenKind::KwSwitch: return "switch";
    case TokenKind::KwCase: return "case";
    case TokenKind::KwDefault: return "default";
    case TokenKind::KwThrow: return "throw";
    case TokenKind::KwThrows: return "throws";
    case TokenKind::KwCatch: return "catch";
    case TokenKind::KwAsync: return "async";
    case TokenKind::KwAwait: return "await";
    case TokenKind::KwTask: return "task";
    case TokenKind::KwActor: return "actor";
    case TokenKind::KwSome: return "some";
    case TokenKind::KwAny: return "any";
    case TokenKind::KwSelf: return "self";
    case TokenKind::KwWhere: return "where";
    case TokenKind::KwGet: return "get";
    case TokenKind::KwSet: return "set";
    case TokenKind::KwMutating: return "mutating";
    case TokenKind::KwOperator: return "operator";
    case TokenKind::KwSubscript: return "subscript";
    case TokenKind::KwYield: return "yield";
    case TokenKind::KwMacro: return "macro";
    case TokenKind::KwAttribute: return "attribute";
    case TokenKind::KwObserve: return "observe";
    case TokenKind::KwSynchronize: return "synchronize";
    case TokenKind::KwCompile: return "compile";
    case TokenKind::KwExtern: return "extern";
    case TokenKind::KwOutput: return "output";
    case TokenKind::KwInput: return "input";
    case TokenKind::KwPanic: return "panic";
    case TokenKind::KwLoop: return "loop";
    case TokenKind::KwEndLoop: return "endLoop";
    case TokenKind::KwThen: return "then";
    case TokenKind::KwEndIf: return "endif";
    case TokenKind::KwSource: return "source";
    case TokenKind::KwFile: return "file";
    case TokenKind::KwFunction: return "function";
    case TokenKind::KwProperty: return "property";
    case TokenKind::KwEvent: return "event";
    case TokenKind::KwSignal: return "signal";
    case TokenKind::KwDetached: return "detached";
    case TokenKind::KwIsolated: return "isolated";
    case TokenKind::KwNonisolated: return "nonisolated";
    case TokenKind::KwSendable: return "sendable";
    default:
        return "unknown";
    }
}

} // namespace hyper
