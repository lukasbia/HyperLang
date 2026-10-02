#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
int expressionPrecedence(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::PipePipe: return 10;
  case tok::Kind::AmpersandAmpersand: return 20;
  case tok::Kind::EqualEqual: case tok::Kind::NotEqual: return 30;
  case tok::Kind::Less: case tok::Kind::LessEqual:
  case tok::Kind::Greater: case tok::Kind::GreaterEqual: return 40;
  case tok::Kind::Pipe: return 45; case tok::Kind::Caret: return 46;
  case tok::Kind::Ampersand: return 47;
  case tok::Kind::Plus: case tok::Kind::Minus: return 50;
  case tok::Kind::Star: case tok::Kind::Slash: case tok::Kind::Percent: return 60;
  default: return -1;
  }
}
bool isExpressionNameToken(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::Identifier: case tok::Kind::EscapedIdentifier:
  case tok::Kind::KwSelf: case tok::Kind::KwSuper: case tok::Kind::KwThis:
  case tok::Kind::KwPrint: case tok::Kind::KwOutput: case tok::Kind::KwError:
  case tok::Kind::KwPanic: case tok::Kind::KwMove: case tok::Kind::KwCopy:
  case tok::Kind::KwBorrow: case tok::Kind::KwPlay: return true;
  default: return isRequestKeyword(kind);
  }
}
}
