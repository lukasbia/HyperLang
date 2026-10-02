#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isIfConfigDirective(tok::Kind kind) {
  return kind == tok::Kind::Hash;
}
}
