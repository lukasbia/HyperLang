//===--- ParseDeclName.cpp - Declaration Name Parsing ---------------------===//
#include "hyperlang/Parse/ParseSupport.h"

namespace hyperlang::parse {
bool isDeclarationNameToken(tok::Kind kind) {
  return kind == tok::Kind::Identifier ||
         kind == tok::Kind::EscapedIdentifier ||
         kind == tok::Kind::KwMain;
}
}
