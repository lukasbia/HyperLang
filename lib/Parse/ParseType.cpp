#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isBuiltinTypeToken(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::KwInt: case tok::Kind::KwUInt: case tok::Kind::KwInt8:
  case tok::Kind::KwInt16: case tok::Kind::KwInt32: case tok::Kind::KwInt64:
  case tok::Kind::KwUInt8: case tok::Kind::KwUInt16: case tok::Kind::KwUInt32:
  case tok::Kind::KwUInt64: case tok::Kind::KwFloat: case tok::Kind::KwDouble:
  case tok::Kind::KwString: case tok::Kind::KwBool: case tok::Kind::KwNum:
  case tok::Kind::KwDecimal: case tok::Kind::KwBytes: case tok::Kind::KwArray:
  case tok::Kind::KwMap: case tok::Kind::KwSet: case tok::Kind::KwTuple: return true;
  default: return false;
  }
}
}
