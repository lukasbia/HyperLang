#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
int expressionPrecedence(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::PipePipe: return 10;
  case tok::Kind::AmpersandAmpersand: return 20;
  case tok::Kind::EqualEqual:
  case tok::Kind::NotEqual:
  case tok::Kind::TildeEqual:
    return 30;
  case tok::Kind::Less: case tok::Kind::LessEqual:
  case tok::Kind::Greater: case tok::Kind::GreaterEqual: return 40;
  case tok::Kind::ShiftLeft:
  case tok::Kind::ShiftRight: return 48;
  case tok::Kind::Pipe: return 45;
  case tok::Kind::Caret: return 46;
  case tok::Kind::Ampersand: return 47;
  case tok::Kind::Plus: case tok::Kind::Minus: return 50;
  case tok::Kind::Star: case tok::Kind::Slash: case tok::Kind::Percent: return 60;
  default: return -1;
  }
}
bool isExpressionNameToken(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::Identifier: case tok::Kind::EscapedIdentifier:
  case tok::Kind::KwMain:
  case tok::Kind::KwSelf: case tok::Kind::KwSuper: case tok::Kind::KwThis:
  case tok::Kind::KwPrint: case tok::Kind::KwOutput: case tok::Kind::KwError:
  case tok::Kind::KwPanic: case tok::Kind::KwMove: case tok::Kind::KwCopy:
  case tok::Kind::KwBorrow: case tok::Kind::KwPlay: return true;
  default: return isRequestKeyword(kind);
  }
}
}

bool isAssignmentOperator(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::Equal:
  case tok::Kind::PlusEqual:
  case tok::Kind::MinusEqual:
  case tok::Kind::StarEqual:
  case tok::Kind::SlashEqual:
  case tok::Kind::PercentEqual:
  case tok::Kind::AmpersandEqual:
  case tok::Kind::PipeEqual:
  case tok::Kind::CaretEqual:
  case tok::Kind::ShiftLeftEqual:
  case tok::Kind::ShiftRightEqual:
    return true;
  default:
    return false;
  }
}

bool isUnaryOperator(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::Plus:
  case tok::Kind::Minus:
  case tok::Kind::Bang:
  case tok::Kind::Tilde:
  case tok::Kind::Increment:
  case tok::Kind::Decrement:
    return true;
  default:
    return false;
  }
}
