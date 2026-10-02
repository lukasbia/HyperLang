#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isPatternToken(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::Identifier: case tok::Kind::EscapedIdentifier:
  case tok::Kind::KwTrue: case tok::Kind::KwFalse: case tok::Kind::KwNil:
  case tok::Kind::KwNull: case tok::Kind::IntegerLiteral:
  case tok::Kind::StringLiteral: case tok::Kind::CharacterLiteral:
  case tok::Kind::OpenParen: case tok::Kind::OpenBracket: return true;
  default: return false;
  }
}
}
