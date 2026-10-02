#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isStatementStarter(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::KwIf: case tok::Kind::KwWhile: case tok::Kind::KwFor:
  case tok::Kind::KwGuard: case tok::Kind::KwDefer: case tok::Kind::KwSwitch:
  case tok::Kind::KwReturn: case tok::Kind::KwBreak: case tok::Kind::KwContinue:
  case tok::Kind::OpenBrace: case tok::Kind::OpenParen: case tok::Kind::OpenBracket:
  case tok::Kind::Identifier: case tok::Kind::EscapedIdentifier:
  case tok::Kind::IntegerLiteral: case tok::Kind::FloatingLiteral:
  case tok::Kind::StringLiteral: case tok::Kind::CharacterLiteral:
  case tok::Kind::KwTrue: case tok::Kind::KwFalse: case tok::Kind::KwNil:
  case tok::Kind::KwNull: return true;
  default: return isExpressionNameToken(kind);
  }
}
}
