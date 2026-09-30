#include "hyper/Lexer.h"

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

struct UTF8Result {
    std::uint32_t codePoint = 0;
    std::size_t width = 0;
    bool valid = false;
};

constexpr std::uint32_t ReplacementCharacter = 0xFFFD;

UTF8Result decodeUTF8(std::string_view source, std::size_t offset) {
    if (offset >= source.size()) return {};
    const unsigned char first = static_cast<unsigned char>(source[offset]);
    if (first < 0x80) return {first, 1, true};
    std::size_t width = 0;
    std::uint32_t value = 0;
    if ((first & 0xE0) == 0xC0) { width = 2; value = first & 0x1F; }
    else if ((first & 0xF0) == 0xE0) { width = 3; value = first & 0x0F; }
    else if ((first & 0xF8) == 0xF0) { width = 4; value = first & 0x07; }
    else return {};
    if (offset + width > source.size()) return {};
    for (std::size_t i = 1; i < width; ++i) {
        const unsigned char next = static_cast<unsigned char>(source[offset + i]);
        if ((next & 0xC0) != 0x80) return {};
        value = (value << 6) | (next & 0x3F);
    }
    if ((width == 2 && value < 0x80) || (width == 3 && value < 0x800) || (width == 4 && value < 0x10000)) return {};
    if (value > 0x10FFFF || (value >= 0xD800 && value <= 0xDFFF)) return {};
    return {value, width, true};
}

bool validScalar(std::uint32_t value) noexcept {
    return value <= 0x10FFFF && !(value >= 0xD800 && value <= 0xDFFF);
}

bool identifierContinue(std::uint32_t value) noexcept {
    if (value < 0x80) return std::isalnum(static_cast<unsigned char>(value)) != 0 || value == "_";
    if (value == 0x00A8 || value == 0x00AA || value == 0x00AD || value == 0x00AF || value == 0x00B5 || value == 0x00B7 || value == 0x00BA || value == 0x200C || value == 0x200D || value == 0x203F || value == 0x2040 || value == 0x2054) return true;
    return (value >= 0x00C0 && value <= 0x02FF) || (value >= 0x0370 && value <= 0x052F) || (value >= 0x0531 && value <= 0x058F) || (value >= 0x0600 && value <= 0x06FF) || (value >= 0x0700 && value <= 0x074F) || (value >= 0x0780 && value <= 0x07BF) || (value >= 0x0900 && value <= 0x0DFF) || (value >= 0x0E00 && value <= 0x0FFF) || (value >= 0x1000 && value <= 0x1FFF) || (value >= 0x2000 && value <= 0x2FFF) || (value >= 0x3040 && value <= 0xD7FF) || (value >= 0xF900 && value <= 0xFAFF) || (value >= 0xFE70 && value <= 0xFEFF) || (value >= 0x10000 && value <= 0xEFFFD);
}

bool identifierStart(std::uint32_t value) noexcept {
    if (value < 0x80) return std::isalpha(static_cast<unsigned char>(value)) != 0 || value == "_";
    if (!identifierContinue(value)) return false;
    return !(value >= 0x0300 && value <= 0x036F) && !(value >= 0x1DC0 && value <= 0x1DFF) && !(value >= 0x20D0 && value <= 0x20FF) && !(value >= 0xFE20 && value <= 0xFE2F);
}

bool whitespace(std::uint32_t value) noexcept {
    switch (value) {
    case 0x0009: case 0x000A: case 0x000B: case 0x000C: case 0x000D: case 0x0020: case 0x0085: case 0x00A0: case 0x1680:
    case 0x2000: case 0x2001: case 0x2002: case 0x2003: case 0x2004: case 0x2005: case 0x2006: case 0x2007: case 0x2008: case 0x2009: case 0x200A:
    case 0x2028: case 0x2029: case 0x202F: case 0x205F: case 0x3000: return true;
    default: return false;
    }
}

bool zeroWidth(std::uint32_t value) noexcept {
    return value == 0x200B || value == 0x200C || value == 0x200D || value == 0x2060 || value == 0xFEFF;
}

bool asciiIdentifierStart(char value) noexcept {
    return std::isalpha(static_cast<unsigned char>(value)) != 0 || value == "_";
}

bool asciiIdentifierContinue(char value) noexcept {
    return std::isalnum(static_cast<unsigned char>(value)) != 0 || value == "_";
}

bool operatorCharacter(char value) noexcept {
    switch (value) {
    case "=": case "-": case "+": case "*": case "/": case "%": case "&": case "|": case "^": case "~": case "!": case "?": case "<": case ">": case ".": return true;
    default: return false;
    }
}

bool horizontalSpace(char value) noexcept {
    return value == " " || value == "\t" || value == "\f" || value == "\v";
}

bool beforeOperatorDelimiter(char value) noexcept {
    return horizontalSpace(value) || value == "\r" || value == "\n" || value == "(" || value == "[" || value == "{" || value == "," || value == ";" || value == ":";
}

bool afterOperatorDelimiter(char value) noexcept {
    return horizontalSpace(value) || value == "\r" || value == "\n" || value == ")" || value == "]" || value == "}" || value == "," || value == ";";
}

std::size_t hashCount(std::string_view source, std::size_t position) {
    std::size_t count = 0;
    while (position + count < source.size() && source[position + count] == "#") ++count;
    return count;
}

const std::unordered_map<std::string_view, TokenKind> &keywords() {
    static const std::unordered_map<std::string_view, TokenKind> table = {
        {"if",TokenKind::KwIf},{"else",TokenKind::KwElse},{"while",TokenKind::KwWhile},{"do",TokenKind::KwDo},{"for",TokenKind::KwFor},{"in",TokenKind::KwIn},
        {"func",TokenKind::KwFunc},{"var",TokenKind::KwVar},{"let",TokenKind::KwLet},{"const",TokenKind::KwConst},{"return",TokenKind::KwReturn},
        {"struct",TokenKind::KwStruct},{"class",TokenKind::KwClass},{"enum",TokenKind::KwEnum},{"protocol",TokenKind::KwProtocol},{"extension",TokenKind::KwExtension},
        {"import",TokenKind::KwImport},{"module",TokenKind::KwModule},{"package",TokenKind::KwPackage},{"namespace",TokenKind::KwNamespace},
        {"public",TokenKind::KwPublic},{"private",TokenKind::KwPrivate},{"internal",TokenKind::KwInternal},{"protected",TokenKind::KwProtected},
        {"static",TokenKind::KwStatic},{"final",TokenKind::KwFinal},{"override",TokenKind::KwOverride},{"init",TokenKind::KwInit},{"deinit",TokenKind::KwDeinit},
        {"defer",TokenKind::KwDefer},{"break",TokenKind::KwBreak},{"continue",TokenKind::KwContinue},{"true",TokenKind::KwTrue},{"false",TokenKind::KwFalse},{"nil",TokenKind::KwNil},
        {"and",TokenKind::KwAnd},{"or",TokenKind::KwOr},{"not",TokenKind::KwNot},{"move",TokenKind::KwMove},{"borrow",TokenKind::KwBorrow},{"consume",TokenKind::KwConsume},
        {"copy",TokenKind::KwCopy},{"own",TokenKind::KwOwn},{"shared",TokenKind::KwShared},{"weak",TokenKind::KwWeak},{"unowned",TokenKind::KwUnowned},{"auto",TokenKind::KwAuto},
        {"type",TokenKind::KwType},{"typealias",TokenKind::KwTypealias},{"associatedtype",TokenKind::KwAssociatedType},
        {"int",TokenKind::KwInt},{"float",TokenKind::KwFloat},{"bool",TokenKind::KwBool},{"string",TokenKind::KwString},{"bytes",TokenKind::KwBytes},{"num",TokenKind::KwNum},
        {"guard",TokenKind::KwGuard},{"switch",TokenKind::KwSwitch},{"case",TokenKind::KwCase},{"default",TokenKind::KwDefault},
        {"throw",TokenKind::KwThrow},{"throws",TokenKind::KwThrows},{"catch",TokenKind::KwCatch},
        {"async",TokenKind::KwAsync},{"await",TokenKind::KwAwait},{"task",TokenKind::KwTask},{"actor",TokenKind::KwActor},
        {"some",TokenKind::KwSome},{"any",TokenKind::KwAny},{"self",TokenKind::KwSelf},{"where",TokenKind::KwWhere},{"get",TokenKind::KwGet},{"set",TokenKind::KwSet},
        {"mutating",TokenKind::KwMutating},{"operator",TokenKind::KwOperator},{"subscript",TokenKind::KwSubscript},{"yield",TokenKind::KwYield},
        {"macro",TokenKind::KwMacro},{"attribute",TokenKind::KwAttribute},{"observe",TokenKind::KwObserve},{"synchronize",TokenKind::KwSynchronize},
        {"compile",TokenKind::KwCompile},{"extern",TokenKind::KwExtern},{"output",TokenKind::KwOutput},{"input",TokenKind::KwInput},{"panic",TokenKind::KwPanic},
        {"loop",TokenKind::KwLoop},{"endLoop",TokenKind::KwEndLoop},{"then",TokenKind::KwThen},{"endif",TokenKind::KwEndIf},
        {"source",TokenKind::KwSource},{"file",TokenKind::KwFile},{"function",TokenKind::KwFunction},{"property",TokenKind::KwProperty},
        {"event",TokenKind::KwEvent},{"signal",TokenKind::KwSignal},{"detached",TokenKind::KwDetached},{"isolated",TokenKind::KwIsolated},{"nonisolated",TokenKind::KwNonisolated},{"sendable",TokenKind::KwSendable}
    };
    return table;
}

const std::unordered_map<std::string_view, TokenKind> &operators() {
    static const std::unordered_map<std::string_view, TokenKind> table = {
        {"+",TokenKind::Plus},{"-",TokenKind::Minus},{"*",TokenKind::Star},{"/",TokenKind::Slash},{"%",TokenKind::Percent},
        {"&",TokenKind::Ampersand},{"|",TokenKind::Pipe},{"^",TokenKind::Caret},{"~",TokenKind::Tilde},{"!",TokenKind::Bang},{"?",TokenKind::Question},
        {"=",TokenKind::Equal},{"==",TokenKind::EqualEqual},{"!=",TokenKind::NotEqual},{">",TokenKind::Greater},{">=",TokenKind::GreaterEqual},{"<",TokenKind::Less},{"<=",TokenKind::LessEqual},
        {"+=",TokenKind::PlusEqual},{"-=",TokenKind::MinusEqual},{"*=",TokenKind::StarEqual},{"/=",TokenKind::SlashEqual},{"%=",TokenKind::PercentEqual},
        {"&=",TokenKind::AmpersandEqual},{"|=",TokenKind::PipeEqual},{"^=",TokenKind::CaretEqual},{"->",TokenKind::Arrow},{"=>",TokenKind::FatArrow},
        {"..",TokenKind::Range},{"...",TokenKind::ClosedRange},{"??",TokenKind::NilCoalescing},{"**",TokenKind::Power},{"<<",TokenKind::ShiftLeft},{">>",TokenKind::ShiftRight},
        {"<<=",TokenKind::ShiftLeftEqual},{">>=",TokenKind::ShiftRightEqual},{"&&",TokenKind::AndAnd},{"||",TokenKind::OrOr},{"~=",TokenKind::TildeEqual},{"::",TokenKind::DoubleColon}
    };
    return table;
}

} // namespace

Lexer::Lexer(std::string_view source) : source_(source) {
    if (source_.size() >= 3 && static_cast<unsigned char>(source_[0]) == 0xEF && static_cast<unsigned char>(source_[1]) == 0xBB && static_cast<unsigned char>(source_[2]) == 0xBF) current_ = 3;
}

char Lexer::peek(std::size_t distance) const noexcept {
    const std::size_t position = current_ + distance;
    return position < source_.size() ? source_[position] : "\0";
}

char Lexer::advance() noexcept {
    if (current_ >= source_.size()) return "\0";
    const char value = source_[current_++];
    if (value == "\r") {
        if (current_ < source_.size() && source_[current] == "\n") ++current_;
        ++line_; column_ = 1; atStartOfLine_ = true; return value;
    }
    if (value == "\n") { ++line_; column_ = 1; atStartOfLine_ = true; return value; }
    ++column_;
    if (!horizontalSpace(value)) atStartOfLine_ = false;
    return value;
}

bool Lexer::match(char expected) noexcept {
    if (peek() != expected) return false;
    advance();
    return true;
}

bool Lexer::startsWith(std::string_view text) const noexcept {
    return current_ <= source_.size() && source_.substr(current_, text.size()) == text;
}

SourceLocation Lexer::locationFromOffset(std::size_t offset) const {
    SourceLocation result{std::min(offset, source_.size()), 1, 1};
    for (std::size_t index = 0; index < result.offset; ++index) {
        if (source_[index] == "\r") {
            if (index + 1 < result.offset && source_[index + 1] == "\n") ++index;
            ++result.line; result.column = 1;
        } else if (source_[index] == "\n") {
            ++result.line; result.column = 1;
        } else {
            ++result.column;
        }
    }
    return result;
}

Token Lexer::makeToken(TokenKind kind, std::size_t start, SourceLocation location, bool malformed) const {
    Token token;
    token.kind = kind;
    token.text = std::string(source_.substr(start, current_ - start));
    token.location = location;
    token.binding = OperatorBinding::None;
    token.atStartOfLine = atStartOfLine_;
    token.malformed = malformed;
    token.hasLeadingComment = hadLeadingComment_;
    return token;
}

void Lexer::diagnose(LexerDiagnostic::Severity severity, SourceLocation location, std::string message, std::string replacement) {
    diagnostics_.push_back({severity, location, std::move(message), std::move(replacement)});
}

void Lexer::diagnoseError(SourceLocation location, std::string message, std::string replacement) {
    diagnose(LexerDiagnostic::Severity::Error, location, std::move(message), std::move(replacement));
}

void Lexer::diagnoseWarning(SourceLocation location, std::string message, std::string replacement) {
    diagnose(LexerDiagnostic::Severity::Warning, location, std::move(message), std::move(replacement));
}

void Lexer::reset() {
    current_ = 0; line_ = 1; column_ = 1; atStartOfLine_ = true;
    hadLeadingComment_ = false; leadingComment_.clear(); lookaheadBuffer_.clear();
    if (source_.size() >= 3 && static_cast<unsigned char>(source_[0]) == 0xEF && static_cast<unsigned char>(source_[1]) == 0xBB && static_cast<unsigned char>(source_[2]) == 0xBF) current_ = 3;
}

void Lexer::setKeepComments(bool keep) { if (keepComments_ != keep) { keepComments_ = keep; invalidateLookahead(); } }
void Lexer::setAllowHashbang(bool allow) { if (allowHashbang_ != allow) { allowHashbang_ = allow; invalidateLookahead(); } }
void Lexer::setAllowRegexLiterals(bool allow) { if (allowRegexLiterals_ != allow) { allowRegexLiterals_ = allow; invalidateLookahead(); } }
void Lexer::setTreatEditorPlaceholdersAsTokens(bool enabled) { if (treatEditorPlaceholdersAsTokens_ != enabled) { treatEditorPlaceholdersAsTokens_ = enabled; invalidateLookahead(); } }

bool Lexer::keepComments() const noexcept { return keepComments_; }
bool Lexer::allowHashbang() const noexcept { return allowHashbang_; }
bool Lexer::allowRegexLiterals() const noexcept { return allowRegexLiterals_; }
bool Lexer::atEnd() const noexcept { return current_ >= source_.size(); }
std::size_t Lexer::currentOffset() const noexcept { return current_; }
SourceLocation Lexer::currentLocation() const noexcept { return {current_, line_, column_}; }
const std::vector<LexerDiagnostic> &Lexer::diagnostics() const noexcept { return diagnostics_; }
void Lexer::clearDiagnostics() { diagnostics_.clear(); }

bool Lexer::isWhitespaceAt(std::size_t offset) const noexcept {
    if (offset >= source_.size()) return false;
    if (std::isspace(static_cast<unsigned char>(source_[offset])) != 0) return true;
    const UTF8Result decoded = decodeUTF8(source_, offset);
    return decoded.valid && whitespace(decoded.codePoint);
}

bool Lexer::isLineBreakAt(std::size_t offset) const noexcept {
    return offset < source_.size() && (source_[offset] == "\r" || source_[offset] == "\n");
}

bool Lexer::lexLineComment() {
    if (!startsWith("//")) return false;
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    const bool doc = startsWith("///") || startsWith("//!");
    advance(); advance();
    while (!atEnd() && !isLineBreakAt(current_)) advance();
    hadLeadingComment_ = true;
    if (!keepComments_) { leadingComment_.append(source_.substr(start, current_ - start)); return true; }
    lookaheadBuffer_.push_back(makeToken(doc ? TokenKind::DocComment : TokenKind::Comment, start, location));
    return true;
}

bool Lexer::lexBlockComment() {
    if (!startsWith("/*")) return false;
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    const bool doc = startsWith("/**") || startsWith("/*!");
    advance(); advance();
    unsigned depth = 1;
    while (!atEnd() && depth != 0) {
        if (startsWith("/*")) { advance(); advance(); ++depth; continue; }
        if (startsWith("*/")) { advance(); advance(); --depth; continue; }
        advance();
    }
    if (depth != 0) diagnoseError(location, "unterminated block comment", "*/");
    hadLeadingComment_ = true;
    if (!keepComments_) { leadingComment_.append(source_.substr(start, current_ - start)); return true; }
    lookaheadBuffer_.push_back(makeToken(doc ? TokenKind::DocComment : TokenKind::Comment, start, location, depth != 0));
    return true;
}

bool Lexer::lexHashbang() {
    if (!startsWith("#!")) return false;
    if (current_ != 0 && current_ != 3) return false;
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    if (!allowHashbang_) diagnoseError(location, "hashbang is not allowed here");
    advance(); advance();
    while (!atEnd() && !isLineBreakAt(current_)) advance();
    hadLeadingComment_ = true;
    if (!keepComments_) { leadingComment_.append(source_.substr(start, current_ - start)); return true; }
    lookaheadBuffer_.push_back(makeToken(TokenKind::Hashbang, start, location));
    return true;
}

bool Lexer::lexConflictMarker() {
    if (!atStartOfLine_ || (!startsWith("<<<<<<< ") && !startsWith(">>>>>>> "))) return false;
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    diagnoseError(location, "unresolved source control conflict marker");
    while (!atEnd() && !isLineBreakAt(current_)) advance();
    if (keepComments_) lookaheadBuffer_.push_back(makeToken(TokenKind::ConflictMarker, start, location));
    return true;
}

void Lexer::consumeTrivia() {
    hadLeadingComment_ = false;
    leadingComment_.clear();
    for (;;) {
        bool consumed = false;
        while (!atEnd() && isWhitespaceAt(current_)) { advance(); consumed = true; }
        if (atEnd()) return;
        if (startsWith("//")) { lexLineComment(); consumed = true; if (keepComments_ && !lookaheadBuffer_.empty()) return; continue; }
        if (startsWith("/*")) { lexBlockComment(); consumed = true; if (keepComments_ && !lookaheadBuffer_.empty()) return; continue; }
        if ((current_ == 0 || current_ == 3) && startsWith("#!")) { lexHashbang(); consumed = true; if (keepComments_ && !lookaheadBuffer_.empty()) return; continue; }
        if (atStartOfLine_ && (startsWith("<<<<<<< ") || startsWith(">>>>>>> "))) { lexConflictMarker(); consumed = true; if (keepComments_ && !lookaheadBuffer_.empty()) return; continue; }
        if (!consumed) return;
    }
}

Token Lexer::lexIdentifierOrKeyword() {
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    const UTF8Result first = decodeUTF8(source_, current_);
    if (!first.valid || !identifierStart(first.codePoint)) return lexUnknown();
    current_ += first.width; column_ += first.width;
    while (!atEnd()) {
        const UTF8Result next = decodeUTF8(source_, current_);
        if (!next.valid || !identifierContinue(next.codePoint)) break;
        current_ += next.width; column_ += next.width;
    }
    const std::string_view spelling = source_.substr(start, current_ - start);
    const auto found = keywords().find(spelling);
    return makeToken(found == keywords().end() ? TokenKind::Identifier : found->second, start, location);
}

Token Lexer::lexEscapedIdentifier() {
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    advance();
    const std::size_t bodyStart = current_;
    while (!atEnd() && peek() != "`") {
        if (isLineBreakAt(current_)) { diagnoseError(currentLocation(), "escaped identifier cannot contain a newline"); break; }
        const UTF8Result decoded = decodeUTF8(source_, current_);
        if (!decoded.valid) { diagnoseError(currentLocation(), "invalid UTF-8 in escaped identifier"); advance(); continue; }
        if (zeroWidth(decoded.codePoint)) diagnoseWarning(currentLocation(), "invisible character in escaped identifier");
        current_ += decoded.width; column_ += decoded.width;
    }
    if (peek() != "`") return makeToken(TokenKind::Unknown, start, location, true);
    const std::size_t bodyEnd = current_;
    advance();
    const std::string_view body = source_.substr(bodyStart, bodyEnd - bodyStart);
    if (body.empty() || isEscapedIdentifierEntirelyWhitespace(body)) diagnoseError(location, "escaped identifier must contain visible characters");
    Token token = makeToken(TokenKind::EscapedIdentifier, start, location);
    token.escapedIdentifier = true;
    return token;
}

Token Lexer::lexDollarIdentifier() {
    const std::size_t start = current_;
    const SourceLocation location = currentLocation();
    advance();
    bool digits = true;
    while (!atEnd() && asciiIdentifierContinue(peek())) { if (!isASCIIDigit(peek())) digits = false; advance(); }
    return makeToken(digits ? TokenKind::DollarIdentifier : TokenKind::Identifier, start, location);
}

bool Lexer::scanDecimalDigits(std::size_t &position, bool allowUnderscores, std::size_t &count, bool &hadSeparators) {
    count = 0; hadSeparators = false; bool previousDigit = false;
    while (position < source_.size()) {
        const char value = source_[position];
        if (isASCIIDigit(value)) { ++position; ++count; previousDigit = true; continue; }
        if (value == "_" && allowUnderscores) {
            const bool validNext = position + 1 < source_.size() && isASCIIDigit(source_[position + 1]);
            if (!previousDigit || !validNext) { diagnoseError(locationFromOffset(position), "invalid separator in numeric literal"); break; }
            hadSeparators = true; previousDigit = false; ++position; continue;
        }
        break;
    }
    return count != 0;
}

bool Lexer::scanBasedDigits(std::size_t &position, NumberBase base, std::size_t &count, bool &hadSeparators) {
    count = 0; hadSeparators = false; bool previousDigit = false;
    const auto valid = [base](char value) {
        if (base == NumberBase::Binary) return value == "0" || value == "1";
        if (base == NumberBase::Octal) return value >= "0" && value <= "7";
        if (base == NumberBase::Decimal) return std::isdigit(static_cast<unsigned char>(value)) != 0;
        return std::isxdigit(static_cast<unsigned char>(value)) != 0;
    };
    while (position < source_.size()) {
        const char value = source_[position];
        if (valid(value)) { ++position; ++count; previousDigit = true; continue; }
        if (value == "_") {
            const bool validNext = position + 1 < source_.size() && valid(source_[position + 1]);
            if (!previousDigit || !validNext) { diagnoseError(locationFromOffset(position), "invalid separator in numeric literal"); break; }
            hadSeparators = true; previousDigit = false; ++position; continue;
        }
        break;
    }
    return count != 0;
}

Token Lexer::lexBinaryNumber() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    advance(); advance(); std::size_t count = 0; bool separators = false;
    const bool valid = scanBasedDigits(current_, NumberBase::Binary, count, separators);
    if (!valid) diagnoseError(currentLocation(), "binary integer literal requires digits");
    Token token = makeToken(TokenKind::BinaryIntegerLiteral, start, location, !valid);
    token.numeric.base = NumberBase::Binary; token.numeric.hasSeparators = separators; return token;
}

Token Lexer::lexOctalNumber() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    advance(); advance(); std::size_t count = 0; bool separators = false;
    const bool valid = scanBasedDigits(current_, NumberBase::Octal, count, separators);
    if (!valid) diagnoseError(currentLocation(), "octal integer literal requires digits");
    Token token = makeToken(TokenKind::OctalIntegerLiteral, start, location, !valid);
    token.numeric.base = NumberBase::Octal; token.numeric.hasSeparators = separators; return token;
}

Token Lexer::lexHexNumber() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    advance(); advance(); std::size_t count = 0; bool separators = false;
    if (!scanBasedDigits(current_, NumberBase::Hexadecimal, count, separators)) {
        diagnoseError(currentLocation(), "hexadecimal literal requires digits");
        Token token = makeToken(TokenKind::HexIntegerLiteral, start, location, true); token.numeric.base = NumberBase::Hexadecimal; return token;
    }
    bool point = false; bool exponent = false; bool malformed = false;
    if (peek() == "." && isASCIIHexDigit(peek(1))) {
        point = true; advance(); std::size_t fraction = 0; bool fractionSeparators = false;
        scanBasedDigits(current_, NumberBase::Hexadecimal, fraction, fractionSeparators); separators = separators || fractionSeparators;
    }
    if (peek() == "p" || peek() == "P") {
        exponent = true; advance(); if (peek() == "+" || peek() == "-") advance();
        std::size_t exponentCount = 0; bool exponentSeparators = false;
        if (!scanDecimalDigits(current_, true, exponentCount, exponentSeparators)) { diagnoseError(currentLocation(), "hexadecimal floating literal requires an exponent"); malformed = true; }
        separators = separators || exponentSeparators;
    } else if (point) { diagnoseError(currentLocation(), "hexadecimal floating literal requires p exponent"); malformed = true; }
    Token token = makeToken(point || exponent ? TokenKind::HexFloatLiteral : TokenKind::HexIntegerLiteral, start, location, malformed);
    token.numeric.base = NumberBase::Hexadecimal; token.numeric.hasDecimalPoint = point; token.numeric.hasExponent = exponent; token.numeric.exponentIsBinary = exponent; token.numeric.hasSeparators = separators; return token;
}

Token Lexer::lexDecimalNumber() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    std::size_t count = 0; bool separators = false; scanDecimalDigits(current_, true, count, separators);
    bool point = false; bool exponent = false; bool malformed = false;
    if (peek() == "." && peek(1) != "." && isASCIIDigit(peek(1))) { point = true; advance(); std::size_t fraction = 0; bool fractionSeparators = false; scanDecimalDigits(current_, true, fraction, fractionSeparators); separators = separators || fractionSeparators; }
    if (peek() == "e" || peek() == "E") {
        exponent = true; advance(); if (peek() == "+" || peek() == "-") advance();
        std::size_t exponentCount = 0; bool exponentSeparators = false;
        if (!scanDecimalDigits(current_, true, exponentCount, exponentSeparators)) { diagnoseError(currentLocation(), "floating literal requires exponent digits"); malformed = true; }
        separators = separators || exponentSeparators;
    }
    Token token = makeToken(point || exponent ? TokenKind::FloatLiteral : TokenKind::IntegerLiteral, start, location, malformed);
    token.numeric.base = NumberBase::Decimal; token.numeric.hasDecimalPoint = point; token.numeric.hasExponent = exponent; token.numeric.hasSeparators = separators; return token;
}

Token Lexer::lexNumber() {
    if (startsWith("0b") || startsWith("0B")) return lexBinaryNumber();
    if (startsWith("0o") || startsWith("0O")) return lexOctalNumber();
    if (startsWith("0x") || startsWith("0X")) return lexHexNumber();
    return lexDecimalNumber();
}

bool Lexer::scanUnicodeEscape(std::size_t &position, std::uint32_t &value) {
    if (position >= source_.size() || source_[position] != "{") { diagnoseError(locationFromOffset(position), "Unicode escape must use the form \\u{...}"); return false; }
    ++position; value = 0; unsigned digits = 0;
    while (position < source_.size() && isASCIIHexDigit(source_[position])) {
        const char digit = source_[position++]; value <<= 4;
        if (digit >= "0" && digit <= "9") value |= static_cast<unsigned>(digit - "0");
        else if (digit >= "a" && digit <= "f") value |= static_cast<unsigned>(digit - "a" + 10);
        else value |= static_cast<unsigned>(digit - "A" + 10);
        if (++digits > 8) break;
    }
    if (position >= source_.size() || source_[position] != "}") { diagnoseError(locationFromOffset(position), "Unicode escape is missing its closing brace"); return false; }
    ++position;
    if (digits == 0 || digits > 8 || !validScalar(value)) { diagnoseError(locationFromOffset(position), "invalid Unicode scalar escape"); return false; }
    return true;
}

bool Lexer::scanEscapeSequence(std::size_t &position, std::string &decoded, bool &valid) {
    valid = true; if (position >= source_.size() || source_[position] != "\\") return false;
    const std::size_t start = position++;
    if (position >= source_.size()) { diagnoseError(locationFromOffset(start), "unterminated escape sequence"); valid = false; return true; }
    const char escaped = source_[position++];
    switch (escaped) {
    case "0": decoded.push_back("\0"); return true;
    case "n": decoded.push_back("\n"); return true;
    case "r": decoded.push_back("\r"); return true;
    case "t": decoded.push_back("\t"); return true;
    case "b": decoded.push_back("\b"); return true;
    case "f": decoded.push_back("\f"); return true;
    case "v": decoded.push_back("\v"); return true;
    case "\\": decoded.push_back("\\"); return true;
    case "": decoded.push_back(""); return true;
    case "'": decoded.push_back("'"); return true;
    case "u": { std::uint32_t scalar = 0; if (!scanUnicodeEscape(position, scalar)) { valid = false; return true; } if (!encodeUTF8(scalar, decoded)) valid = false; return true; }
    case "\n": return true;
    case "\r": if (position < source_.size() && source_[position] == "\n") ++position; return true;
    default: diagnoseError(locationFromOffset(start), "unknown escape sequence"); valid = false; decoded.push_back(escaped); return true;
    }
}

bool Lexer::scanStringDelimiter(std::size_t &delimiterLength, bool &multiline) {
    delimiterLength = 0; multiline = false; if (peek() != """) return false;
    if (peek(1) == """ && peek(2) == """) { delimiterLength = 3; multiline = true; return true; }
    delimiterLength = 1; return true;
}

bool Lexer::scanInterpolatedExpression(std::size_t openingOffset, std::size_t &closingOffset) {
    if (openingOffset >= source_.size()) return false;
    std::size_t position = current_; unsigned parens = 1; unsigned braces = 0; unsigned brackets = 0;
    bool escaped = false; bool single = false; bool double = false;
    while (position < source_.size()) {
        const char value = source_[position];
        if (escaped) { escaped = false; ++position; continue; }
        if (value == "\\") { escaped = true; ++position; continue; }
        if (double) { if (value == """) double = false; ++position; continue; }
        if (single) { if (value == "'") single = false; ++position; continue; }
        switch (value) {
        case """: double = true; ++position; break;
        case "'": single = true; ++position; break;
        case "(": ++parens; ++position; break;
        case ")": if (braces == 0 && brackets == 0) { --parens; if (parens == 0) { closingOffset = position; return true; } } ++position; break;
        case "{": ++braces; ++position; break;
        case "}": if (braces != 0) --braces; ++position; break;
        case "[": ++brackets; ++position; break;
        case "]": if (brackets != 0) --brackets; ++position; break;
        default: ++position; break;
        }
    }
    return false;
}

Token Lexer::lexString() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    std::size_t delimiter = 0; bool multiline = false; scanStringDelimiter(delimiter, multiline);
    for (std::size_t i = 0; i < delimiter; ++i) advance();
    bool interpolation = false; bool escapes = false; bool malformed = false; bool terminated = false;
    while (!atEnd()) {
        if (multiline && startsWith(""""")) { advance(); advance(); advance(); terminated = true; break; }
        if (!multiline && peek() == """) { advance(); terminated = true; break; }
        if (!multiline && isLineBreakAt(current_)) { diagnoseError(location, "string literal cannot cross a line boundary"); malformed = true; break; }
        if (peek() == "\\") {
            if (peek(1) == "(") { interpolation = true; advance(); advance(); std::size_t closing = 0; if (!scanInterpolatedExpression(current_ - 1, closing)) { diagnoseError(currentLocation(), "unterminated string interpolation"); malformed = true; break; } current_ = closing + 1; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; continue; }
            std::size_t position = current_; std::string decoded; bool valid = true; scanEscapeSequence(position, decoded, valid); current_ = position; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; escapes = true; malformed = malformed || !valid; continue;
        }
        const auto decoded = decodeUTF8(source_, current_);
        if (!decoded.valid) { diagnoseError(currentLocation(), "invalid UTF-8 in string literal"); malformed = true; advance(); continue; }
        if (decoded.codePoint < 0x20 && decoded.codePoint != "\t" && decoded.codePoint != "\n" && decoded.codePoint != "\r") { diagnoseError(currentLocation(), "control character in string literal"); malformed = true; }
        current_ += decoded.width; column_ += decoded.width;
    }
    if (!terminated) { diagnoseError(location, "unterminated string literal", multiline ? """"" : """); malformed = true; }
    Token token = makeToken(multiline ? TokenKind::MultilineStringLiteral : TokenKind::StringLiteral, start, location, malformed);
    token.string.kind = multiline ? StringKind::Multiline : StringKind::Normal; token.string.hasInterpolation = interpolation; token.string.hasEscapes = escapes; return token;
}

Token Lexer::lexRawString(unsigned delimiterLength) {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    for (unsigned i = 0; i < delimiterLength; ++i) advance();
    if (peek() != """) { diagnoseError(location, "raw string delimiter is missing its quote"); return makeToken(TokenKind::Hash, start, location, true); }
    bool multiline = startsWith(""""");
    if (multiline) { advance(); advance(); advance(); } else advance();
    bool interpolation = false; bool malformed = false;
    while (!atEnd()) {
        const std::size_t quoteLength = multiline ? 3 : 1;
        if (peek() == "" && current_ + quoteLength <= source_.size()) {
            if ((!multiline || startsWith("""""))) {
                const std::size_t hashStart = current_ + quoteLength; bool closing = true;
                for (unsigned i = 0; i < delimiterLength; ++i) if (hashStart + i >= source_.size() || source_[hashStart + i] != "#") { closing = false; break; }
                if (closing) { current_ = hashStart + delimiterLength; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; break; }
            }
        }
        if (peek() == "\\") {
            std::size_t position = current_ + 1; unsigned hashes = 0; while (hashes < delimiterLength && position < source_.size() && source_[position] == "#") { ++hashes; ++position; }
            if (hashes == delimiterLength && position < source_.size() && source_[position] == "(") {
                interpolation = true; current_ = position + 1; const auto moved = locationFromOffset(current_); line_ = moved.line; column_ = moved.column;
                std::size_t closing = 0; if (!scanInterpolatedExpression(current_ - 1, closing)) { diagnoseError(currentLocation(), "unterminated raw string interpolation"); malformed = true; break; }
                current_ = closing + 1; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; continue;
            }
        }
        advance();
    }
    if (atEnd()) { diagnoseError(location, "unterminated raw string literal"); malformed = true; }
    Token token = makeToken(multiline ? TokenKind::MultilineStringLiteral : TokenKind::StringLiteral, start, location, malformed);
    token.string.kind = multiline ? StringKind::RawMultiline : StringKind::Raw; token.string.customDelimiterLength = delimiterLength; token.string.hasInterpolation = interpolation; return token;
}

Token Lexer::lexCharacter() {
    const std::size_t start = current_; const SourceLocation location = currentLocation(); advance();
    unsigned count = 0; bool malformed = false;
    while (!atEnd() && peek() != "'") {
        if (peek() == "\\") { std::size_t position = current_; std::string decoded; bool valid = true; scanEscapeSequence(position, decoded, valid); current_ = position; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; ++count; malformed = malformed || !valid; continue; }
        const auto decoded = decodeUTF8(source_, current_);
        if (!decoded.valid) { diagnoseError(currentLocation(), "invalid UTF-8 in character literal"); malformed = true; advance(); continue; }
        if (decoded.codePoint == "\n" || decoded.codePoint == "\r") { diagnoseError(currentLocation(), "character literal cannot contain a newline"); malformed = true; break; }
        current_ += decoded.width; column_ += decoded.width; ++count;
    }
    if (peek() == "'") advance(); else { diagnoseError(location, "unterminated character literal", "'"); malformed = true; }
    if (count != 1) { diagnoseError(location, "character literal must contain exactly one character"); malformed = true; }
    return makeToken(TokenKind::CharacterLiteral, start, location, malformed);
}

Token Lexer::lexDirective() {
    const std::size_t start = current_; const SourceLocation location = currentLocation(); advance();
    if (!asciiIdentifierStart(peek())) return makeToken(TokenKind::AtSign, start, location);
    while (!atEnd() && asciiIdentifierContinue(peek())) advance();
    return makeToken(TokenKind::Directive, start, location);
}

Token Lexer::lexHashConstruct() {
    const std::size_t start = current_; const SourceLocation location = currentLocation(); const std::size_t hashes = hashCount(source_, current_); const std::size_t quote = current_ + hashes;
    if (quote < source_.size() && source_[quote] == """) return lexRawString(static_cast<unsigned>(hashes));
    if (quote < source_.size() && asciiIdentifierStart(source_[quote])) { for (std::size_t i = 0; i < hashes; ++i) advance(); while (!atEnd() && asciiIdentifierContinue(peek())) advance(); return makeToken(TokenKind::Directive, start, location); }
    advance(); return makeToken(TokenKind::Hash, start, location);
}

OperatorBinding Lexer::classifyOperatorBinding(std::string_view source, std::size_t start, std::size_t end) noexcept {
    if (start > end || end > source.size()) return OperatorBinding::None;
    bool leftBound = start != 0 && !beforeOperatorDelimiter(source[start - 1]);
    bool rightBound = end < source.size() && !afterOperatorDelimiter(source[end]);
    if (leftBound == rightBound) return leftBound ? OperatorBinding::BinaryUnspaced : OperatorBinding::BinarySpaced;
    return leftBound ? OperatorBinding::Postfix : OperatorBinding::Prefix;
}

Token Lexer::lexOperator() {
    const std::size_t start = current_; const SourceLocation location = currentLocation();
    while (!atEnd() && operatorCharacter(peek())) {
        if (peek() == "." && current_ != start && source_[start] != ".") break;
        if (peek() == "/" && peek(1) == "/" ) break;
        if (peek() == "/" && peek(1) == "*") break;
        advance();
    }
    const std::string_view spelling = source_.substr(start, current_ - start); const auto found = operators().find(spelling);
    Token token = found == operators().end() ? makeToken(TokenKind::Unknown, start, location, true) : makeToken(found->second, start, location);
    if (found == operators().end()) diagnoseError(location, "unknown operator sequence");
    token.binding = classifyOperatorBinding(source_, start, current_); return token;
}

Token Lexer::lexRegex() {
    const std::size_t start = current_; const SourceLocation location = currentLocation(); advance();
    bool escaped = false; bool classBody = false; bool terminated = false;
    while (!atEnd()) {
        const char value = peek();
        if (isLineBreakAt(current_)) break;
        if (escaped) { escaped = false; advance(); continue; }
        if (value == "\\") { escaped = true; advance(); continue; }
        if (value == "[") { classBody = true; advance(); continue; }
        if (value == "]" && classBody) { classBody = false; advance(); continue; }
        if (value == "/" && !classBody) { advance(); while (!atEnd() && asciiIdentifierContinue(peek())) advance(); terminated = true; break; }
        advance();
    }
    if (!terminated) diagnoseError(location, "unterminated regex literal", "/");
    return makeToken(TokenKind::RegexLiteral, start, location, !terminated);
}

bool Lexer::looksLikeEditorPlaceholder(std::string_view text) noexcept { return text.size() >= 4 && text[0] == "<" && text[1] == "#" && text[text.size() - 2] == "#" && text[text.size() - 1] == ">"; }
bool Lexer::looksLikeDirective(std::string_view text) noexcept { return text.size() > 1 && text.front() == "@" && asciiIdentifierStart(text[1]); }
bool Lexer::looksLikeConflictMarker(std::string_view text) noexcept { return text.starts_with("<<<<<<<") || text.starts_with(">>>>>>>"); }

Token Lexer::lexUnknown() {
    const std::size_t start = current_; const SourceLocation location = currentLocation(); const auto decoded = decodeUTF8(source_, current_);
    if (!decoded.valid) { diagnoseError(location, "invalid UTF-8 sequence"); advance(); return makeToken(TokenKind::Unknown, start, location, true); }
    if (decoded.codePoint == 0x00A0) { diagnoseWarning(location, "non-breaking space used as source whitespace", " "); current_ += decoded.width; column_ += decoded.width; return makeToken(TokenKind::Unknown, start, location, true); }
    if (zeroWidth(decoded.codePoint)) { diagnoseError(location, "invisible Unicode character is not allowed here"); current_ += decoded.width; column_ += decoded.width; return makeToken(TokenKind::Unknown, start, location, true); }
    current_ += decoded.width; column_ += decoded.width; diagnoseError(location, "invalid character in source"); return makeToken(TokenKind::Unknown, start, location, true);
}

Token Lexer::next() {
    if (!lookaheadBuffer_.empty()) { Token result = std::move(lookaheadBuffer_.front()); lookaheadBuffer_.erase(lookaheadBuffer_.begin()); return result; }
    consumeTrivia();
    if (!lookaheadBuffer_.empty()) { Token result = std::move(lookaheadBuffer_.front()); lookaheadBuffer_.erase(lookaheadBuffer_.begin()); return result; }
    if (atEnd()) { Token eof; eof.kind = TokenKind::EndOfFile; eof.location = currentLocation(); eof.atStartOfLine = atStartOfLine_; return eof; }
    const std::size_t start = current_; const SourceLocation location = currentLocation(); const char value = peek();
    if (startsWith("<#") && treatEditorPlaceholdersAsTokens_) { const std::size_t end = source_.find("#>", current_ + 2); if (end != std::string_view::npos && (source_.find("\n", current_) == std::string_view::npos || end < source_.find("\n", current_))) { current_ = end + 2; const auto after = locationFromOffset(current_); line_ = after.line; column_ = after.column; return makeToken(TokenKind::EditorPlaceholder, start, location); } }
    if (value == "@") return lexDirective();
    if (value == "#") return lexHashConstruct();
    if (value == "`") return lexEscapedIdentifier();
    if (value == "$") return lexDollarIdentifier();
    if (value == """) return lexString();
    if (value == "'") return lexCharacter();
    if (isASCIIDigit(value)) return lexNumber();
    if (asciiIdentifierStart(value) || (static_cast<unsigned char>(value) & 0x80) != 0) return lexIdentifierOrKeyword();
    if (value == "/" && allowRegexLiterals_ && peek(1) != "/" && peek(1) != "*") return lexRegex();
    switch (value) {
    case "(": advance(); return makeToken(TokenKind::LeftParen, start, location);
    case ")": advance(); return makeToken(TokenKind::RightParen, start, location);
    case "{": advance(); return makeToken(TokenKind::LeftBrace, start, location);
    case "}": advance(); return makeToken(TokenKind::RightBrace, start, location);
    case "[": advance(); return makeToken(TokenKind::LeftBracket, start, location);
    case "]": advance(); return makeToken(TokenKind::RightBracket, start, location);
    case ",": advance(); return makeToken(TokenKind::Comma, start, location);
    case ";": advance(); return makeToken(TokenKind::Semicolon, start, location);
    case ":": advance(); if (peek() == ":") { advance(); return makeToken(TokenKind::DoubleColon, start, location); } return makeToken(TokenKind::Colon, start, location);
    case "\\": advance(); return makeToken(TokenKind::Backslash, start, location);
    default: break;
    }
    if (operatorCharacter(value)) return lexOperator();
    return lexUnknown();
}

Token Lexer::peek() { return lookahead(0); }

Token Lexer::lookahead(std::size_t distance) {
    while (lookaheadBuffer_.size() <= distance) {
        Token token = next();
        lookaheadBuffer_.push_back(std::move(token));
    }
    return lookaheadBuffer_[distance];
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    tokens.reserve(source_.size() / 2 + 1);
    for (;;) { Token token = next(); tokens.push_back(token); if (token.kind == TokenKind::EndOfFile) break; }
    return tokens;
}

void Lexer::invalidateLookahead() { lookaheadBuffer_.clear(); }

bool Lexer::isIdentifier(std::string_view text) {
    if (text.empty()) return false;
    std::size_t position = 0; const auto first = decodeUTF8(text, position);
    if (!first.valid || !identifierStart(first.codePoint)) return false;
    position += first.width;
    while (position < text.size()) { const auto next = decodeUTF8(text, position); if (!next.valid || !identifierContinue(next.codePoint)) return false; position += next.width; }
    return true;
}

bool Lexer::isOperator(std::string_view text) { if (text.empty()) return false; for (char value : text) if (!operatorCharacter(value)) return false; return true; }

bool Lexer::isValidEscapedIdentifier(std::string_view text) {
    if (text.size() < 2 || text.front() != "`" || text.back() != "`") return false;
    const auto body = text.substr(1, text.size() - 2);
    if (body.empty() || isEscapedIdentifierEntirelyWhitespace(body)) return false;
    std::size_t position = 0; while (position < body.size()) { const auto decoded = decodeUTF8(body, position); if (!decoded.valid || decoded.codePoint == "`" || decoded.codePoint == "\\") return false; position += decoded.width; }
    return true;
}

bool Lexer::isEscapedIdentifierEntirelyWhitespace(std::string_view text) {
    if (text.empty()) return true; std::size_t position = 0;
    while (position < text.size()) { const auto decoded = decodeUTF8(text, position); if (!decoded.valid || !whitespace(decoded.codePoint)) return false; position += decoded.width; }
    return true;
}

bool Lexer::isASCIIIdentifierStart(char value) noexcept { return asciiIdentifierStart(value); }
bool Lexer::isASCIIIdentifierContinue(char value) noexcept { return asciiIdentifierContinue(value); }
bool Lexer::isASCIIDigit(char value) noexcept { return std::isdigit(static_cast<unsigned char>(value)) != 0; }
bool Lexer::isASCIIHexDigit(char value) noexcept { return std::isxdigit(static_cast<unsigned char>(value)) != 0; }
bool Lexer::isASCIIOctalDigit(char value) noexcept { return value >= "0" && value <= "7"; }
bool Lexer::isASCIIWhitespace(char value) noexcept { return std::isspace(static_cast<unsigned char>(value)) != 0; }
bool Lexer::isPrintableASCII(char value) noexcept { return std::isprint(static_cast<unsigned char>(value)) != 0; }
bool Lexer::isUnicodeIdentifierStart(std::uint32_t value) noexcept { return identifierStart(value); }
bool Lexer::isUnicodeIdentifierContinue(std::uint32_t value) noexcept { return identifierContinue(value); }
bool Lexer::isRawIdentifierWhitespace(std::uint32_t value) noexcept { return value == 0x20 || value == 0x200E || value == 0x200F; }
bool Lexer::isForbiddenIdentifierCodePoint(std::uint32_t value) noexcept { return value < 0x20 || value == 0x7F || zeroWidth(value); }
bool Lexer::isOperatorCharacter(char value) noexcept { return operatorCharacter(value); }
bool Lexer::isOperatorStartCharacter(char value) noexcept { return operatorCharacter(value); }
bool Lexer::isOperatorContinuationCharacter(char value) noexcept { return operatorCharacter(value); }

bool Lexer::encodeUTF8(std::uint32_t value, std::string &output) {
    output.clear(); if (!validScalar(value)) return false;
    if (value < 0x80) { output.push_back(static_cast<char>(value)); return true; }
    if (value < 0x800) { output.push_back(static_cast<char>(0xC0 | (value >> 6))); output.push_back(static_cast<char>(0x80 | (value & 0x3F))); return true; }
    if (value < 0x10000) { output.push_back(static_cast<char>(0xE0 | (value >> 12))); output.push_back(static_cast<char>(0x80 | ((value >> 6) & 0x3F))); output.push_back(static_cast<char>(0x80 | (value & 0x3F))); return true; }
    output.push_back(static_cast<char>(0xF0 | (value >> 18))); output.push_back(static_cast<char>(0x80 | ((value >> 12) & 0x3F))); output.push_back(static_cast<char>(0x80 | ((value >> 6) & 0x3F))); output.push_back(static_cast<char>(0x80 | (value & 0x3F))); return true;
}

std::uint32_t Lexer::validateUTF8Character(std::string_view bytes, std::size_t &offset) {
    const auto decoded = decodeUTF8(bytes, offset); if (!decoded.valid) { if (offset < bytes.size()) ++offset; return ReplacementCharacter; } offset += decoded.width; return decoded.codePoint;
}

TokenKind Lexer::keywordKind(std::string_view text) noexcept { const auto found = keywords().find(text); return found == keywords().end() ? TokenKind::Identifier : found->second; }

TokenKind Lexer::punctuationKind(std::string_view text) noexcept {
    if (text == "@") return TokenKind::AtSign; if (text == "#") return TokenKind::Hash; if (text == ".") return TokenKind::Dot; if (text == ",") return TokenKind::Comma; if (text == ":") return TokenKind::Colon; if (text == "::") return TokenKind::DoubleColon; if (text == ";") return TokenKind::Semicolon; if (text == "\\") return TokenKind::Backslash; if (text == "(") return TokenKind::LeftParen; if (text == ")") return TokenKind::RightParen; if (text == "{") return TokenKind::LeftBrace; if (text == "}") return TokenKind::RightBrace; if (text == "[") return TokenKind::LeftBracket; if (text == "]") return TokenKind::RightBracket; return TokenKind::Unknown;
}

TokenKind Lexer::operatorKind(std::string_view text) noexcept { const auto found = operators().find(text); return found == operators().end() ? TokenKind::Unknown : found->second; }

const char *tokenKindName(TokenKind kind) noexcept {
    switch (kind) {
    case TokenKind::EndOfFile: return "eof"; case TokenKind::Unknown: return "unknown"; case TokenKind::Identifier: return "identifier"; case TokenKind::EscapedIdentifier: return "escaped-identifier";
    case TokenKind::DollarIdentifier: return "dollar-identifier"; case TokenKind::IntegerLiteral: return "integer"; case TokenKind::BinaryIntegerLiteral: return "binary-integer"; case TokenKind::OctalIntegerLiteral: return "octal-integer";
    case TokenKind::HexIntegerLiteral: return "hex-integer"; case TokenKind::FloatLiteral: return "float"; case TokenKind::HexFloatLiteral: return "hex-float"; case TokenKind::StringLiteral: return "string";
    case TokenKind::MultilineStringLiteral: return "multiline-string"; case TokenKind::CharacterLiteral: return "character"; case TokenKind::RegexLiteral: return "regex"; case TokenKind::EditorPlaceholder: return "editor-placeholder";
    case TokenKind::AtSign: return "@"; case TokenKind::Hash: return "#"; case TokenKind::Directive: return "directive"; case TokenKind::Plus: return "+"; case TokenKind::Minus: return "-"; case TokenKind::Star: return "*";
    case TokenKind::Slash: return "/"; case TokenKind::Percent: return "%"; case TokenKind::Ampersand: return "&"; case TokenKind::Pipe: return "|"; case TokenKind::Caret: return "^"; case TokenKind::Tilde: return "~";
    case TokenKind::Bang: return "!"; case TokenKind::Question: return "?"; case TokenKind::Equal: return "="; case TokenKind::EqualEqual: return "=="; case TokenKind::NotEqual: return "!=";
    case TokenKind::Greater: return ">"; case TokenKind::GreaterEqual: return ">="; case TokenKind::Less: return "<"; case TokenKind::LessEqual: return "<="; case TokenKind::PlusEqual: return "+=";
    case TokenKind::MinusEqual: return "-="; case TokenKind::StarEqual: return "*="; case TokenKind::SlashEqual: return "/="; case TokenKind::PercentEqual: return "%="; case TokenKind::AmpersandEqual: return "&=";
    case TokenKind::PipeEqual: return "|="; case TokenKind::CaretEqual: return "^="; case TokenKind::Arrow: return "->"; case TokenKind::FatArrow: return "=>"; case TokenKind::Range: return ".."; case TokenKind::ClosedRange: return "...";
    case TokenKind::NilCoalescing: return "??"; case TokenKind::Power: return "**"; case TokenKind::ShiftLeft: return "<<"; case TokenKind::ShiftRight: return ">>"; case TokenKind::ShiftLeftEqual: return "<<="; case TokenKind::ShiftRightEqual: return ">>=";
    case TokenKind::AndAnd: return "&&"; case TokenKind::OrOr: return "||"; case TokenKind::QuestionQuestion: return "??"; case TokenKind::TildeEqual: return "~="; case TokenKind::Dot: return "."; case TokenKind::Comma: return ",";
    case TokenKind::Colon: return ":"; case TokenKind::DoubleColon: return "::"; case TokenKind::Semicolon: return ";"; case TokenKind::Backslash: return "\\"; case TokenKind::LeftParen: return "("; case TokenKind::RightParen: return ")";
    case TokenKind::LeftBrace: return "{"; case TokenKind::RightBrace: return "}"; case TokenKind::LeftBracket: return "["; case TokenKind::RightBracket: return "]"; case TokenKind::Comment: return "comment";
    case TokenKind::DocComment: return "doc-comment"; case TokenKind::Hashbang: return "hashbang"; case TokenKind::ConflictMarker: return "conflict-marker";
    default: return "keyword";
    }
}

} // namespace hyper