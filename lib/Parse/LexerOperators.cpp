#include "hyper/lib/Parse/Lexer.h"
#include <array>
#include <cctype>
namespace hyper::parse {
Token Lexer::lexOperatorOrPunctuation(){
 const auto start=tokenStart_;
 const auto two=[&](std::string_view s,TokenKind k){if(input_.substr(cursor_,s.size())==s){for(char c:s)consume();return makeToken(k,start);}return Token{};};
 switch(peek()){
  case '(':consume();return makeSimple(TokenKind::LParen); case ')':consume();return makeSimple(TokenKind::RParen);
  case '{':consume();return makeSimple(TokenKind::LBrace); case '}':consume();return makeSimple(TokenKind::RBrace);
  case '[':consume();return makeSimple(TokenKind::LBracket); case ']':consume();return makeSimple(TokenKind::RBracket);
  case ',':consume();return makeSimple(TokenKind::Comma); case ':': if(peek(1)==':'){consume();consume();return makeSimple(TokenKind::DoubleColon);} consume();return makeSimple(TokenKind::Colon);
  case ';':consume();return makeSimple(TokenKind::Semicolon);
  case '?': if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::QuestionDot);} if(peek(1)=='?'){consume();consume();return makeSimple(TokenKind::NullCoalescing);} consume();return makeSimple(TokenKind::Question);
  case '.': if(peek(1)=='.'&&peek(2)=='.'){consume();consume();consume();return makeSimple(TokenKind::Ellipsis);} if(peek(1)=='.'&&peek(2)=='='){consume();consume();consume();return makeSimple(TokenKind::RangeInclusive);} if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::Range);} consume();return makeSimple(TokenKind::Dot);
  case '+': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PlusEqual);} if(peek(1)=='+'){consume();consume();return makeSimple(TokenKind::Increment);} consume();return makeSimple(TokenKind::Plus);
  case '-': if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::Arrow);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::MinusEqual);} if(peek(1)=='-'){consume();consume();return makeSimple(TokenKind::Decrement);} consume();return makeSimple(TokenKind::Minus);
  case '*': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::StarEqual);} consume();return makeSimple(TokenKind::Star);
  case '/': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::SlashEqual);} consume();return makeSimple(TokenKind::Slash);
  case '%': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PercentEqual);} consume();return makeSimple(TokenKind::Percent);
  case '&': if(peek(1)=='&'){consume();consume();return makeSimple(TokenKind::AmpAmp);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::AmpEqual);} consume();return makeSimple(TokenKind::Ampersand);
  case '|': if(peek(1)=='|'){consume();consume();return makeSimple(TokenKind::PipePipe);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PipeEqual);} if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::FatArrow);} consume();return makeSimple(TokenKind::Pipe);
  case '^': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::CaretEqual);} consume();return makeSimple(TokenKind::Caret);
  case '~':consume();return makeSimple(TokenKind::Tilde);
  case '!': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::BangEqual);} if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::BangDot);} consume();return makeSimple(TokenKind::Bang);
  case '=': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::EqualEqual);} if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::FatArrow);} consume();return makeSimple(TokenKind::Equal);
  case '<': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::LessEqual);} if(peek(1)=='<'){consume();consume();if(peek()=='='){consume();return makeSimple(TokenKind::ShiftLeftEqual);}return makeSimple(TokenKind::ShiftLeft);} consume();return makeSimple(TokenKind::Less);
  case '>': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::GreaterEqual);} if(peek(1)=='>'){consume();consume();if(peek()=='='){consume();return makeSimple(TokenKind::ShiftRightEqual);}return makeSimple(TokenKind::ShiftRight);} consume();return makeSimple(TokenKind::Greater);
  case '@': {
    while(peek()=='@'||std::isalpha(static_cast<unsigned char>(peek()))||std::isdigit(static_cast<unsigned char>(peek()))||peek()=='_')consume();
    auto spelling=input_.substr(start,cursor_-start);auto k=lookupSpecialKeyword(spelling);
    if(k!=TokenKind::Identifier)return makeToken(k,start);
    error("unknown @-keyword");return makeToken(TokenKind::Unknown,start);
  }
  default: consume();error("unexpected character");return makeToken(TokenKind::Unknown,start);
 }
}
}
