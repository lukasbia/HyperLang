#include "hyper/Parse/LexerToken.h"

namespace hyper {

std::size_t Token::endOffset() const noexcept {
    return location.offset + text.size();
}

bool Token::isIdentifier() const noexcept {
    return kind == TokenKind::Identifier ||
           kind == TokenKind::EscapedIdentifier ||
           kind == TokenKind::DollarIdentifier;
}

bool Token::isLiteral() const noexcept {
    switch (kind) {
        case TokenKind::BinaryIntegerLiteral:
        case TokenKind::OctalIntegerLiteral:
        case TokenKind::HexIntegerLiteral:
        case TokenKind::HexFloatLiteral:
        case TokenKind::FloatLiteral:
        case TokenKind::IntegerLiteral:
        case TokenKind::MultilineStringLiteral:
        case TokenKind::StringLiteral:
        case TokenKind::CharacterLiteral:
        case TokenKind::RegexLiteral:
        case TokenKind::KwTrue:
        case TokenKind::KwFalse:
        case TokenKind::KwNil:
            return true;
        default:
            return false;
    }
}

bool Token::isOperator() const noexcept {
    switch (kind) {
        case TokenKind::Plus:
        case TokenKind::Minus:
        case TokenKind::Star:
        case TokenKind::Slash:
        case TokenKind::Percent:
        case TokenKind::Ampersand:
        case TokenKind::Pipe:
        case TokenKind::Caret:
        case TokenKind::Tilde:
        case TokenKind::Bang:
        case TokenKind::Question:
        case TokenKind::Equal:
        case TokenKind::EqualEqual:
        case TokenKind::NotEqual:
        case TokenKind::Greater:
        case TokenKind::GreaterEqual:
        case TokenKind::Less:
        case TokenKind::LessEqual:
        case TokenKind::PlusEqual:
        case TokenKind::MinusEqual:
        case TokenKind::StarEqual:
        case TokenKind::SlashEqual:
        case TokenKind::PercentEqual:
        case TokenKind::AmpersandEqual:
        case TokenKind::PipeEqual:
        case TokenKind::CaretEqual:
        case TokenKind::Arrow:
        case TokenKind::FatArrow:
        case TokenKind::Range:
        case TokenKind::ClosedRange:
        case TokenKind::NilCoalescing:
        case TokenKind::Power:
        case TokenKind::ShiftLeft:
        case TokenKind::ShiftRight:
        case TokenKind::ShiftLeftEqual:
        case TokenKind::ShiftRightEqual:
        case TokenKind::AndAnd:
        case TokenKind::OrOr:
        case TokenKind::TildeEqual:
        case TokenKind::DoubleColon:
        case TokenKind::QuestionQuestion:
        case TokenKind::Or:
            return true;
        default:
            return false;
    }
}

bool Token::isTrivia() const noexcept {
    return kind == TokenKind::Comment ||
           kind == TokenKind::DocComment ||
           kind == TokenKind::Hashbang;
}

bool Token::isKeyword() const noexcept {
    return static_cast<std::uint16_t>(kind) <=
           static_cast<std::uint16_t>(TokenKind::KwSendable);
}

bool Token::isPunctuation() const noexcept {
    switch (kind) {
        case TokenKind::LeftParen:
        case TokenKind::RightParen:
        case TokenKind::LeftBrace:
        case TokenKind::RightBrace:
        case TokenKind::LeftBracket:
        case TokenKind::RightBracket:
        case TokenKind::Comma:
        case TokenKind::Semicolon:
        case TokenKind::Colon:
        case TokenKind::Backslash:
        case TokenKind::Dot:
        case TokenKind::AtSign:
        case TokenKind::Hash:
            return true;
        default:
            return false;
    }
}

} // namespace hyper
