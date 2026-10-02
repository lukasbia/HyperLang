#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isGenericDelimiter(tok::Kind kind) {
  return kind == tok::Kind::Less || kind == tok::Kind::Greater ||
         kind == tok::Kind::Comma || kind == tok::Kind::Question;
}
}
