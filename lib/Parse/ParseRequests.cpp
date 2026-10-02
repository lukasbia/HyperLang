#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isRequestKeyword(tok::Kind kind) {
  switch (kind) {
  case tok::Kind::KwConnect: case tok::Kind::KwGetData:
  case tok::Kind::KwCreateData: case tok::Kind::KwData:
  case tok::Kind::KwFile: case tok::Kind::KwFileName:
  case tok::Kind::KwBackup: case tok::Kind::KwDelete:
  case tok::Kind::KwDestroy: return true;
  default: return false;
  }
}
}
