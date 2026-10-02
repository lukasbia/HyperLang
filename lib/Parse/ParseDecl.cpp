#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isDeclarationStarter(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::KwFunc: case tok::Kind::KwAsync:
  case tok::Kind::KwVar: case tok::Kind::KwLet: case tok::Kind::KwConst:
  case tok::Kind::KwImport: case tok::Kind::KwStruct: case tok::Kind::KwClass:
  case tok::Kind::KwEnum: case tok::Kind::KwProtocol: case tok::Kind::KwExtension:
    return true;
  default: return false;
  }
}
}
