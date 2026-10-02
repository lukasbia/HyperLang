//===--- Action.cpp - HyperLang Driver Actions ----------------------------===//
#include "Action.h"
namespace hyperlang::driver {
const char *actionKindName(ActionKind kind) {
  switch (kind) {
  case ActionKind::Parse: return "parse";
  case ActionKind::Sema: return "sema";
  case ActionKind::HILGen: return "hilgen";
  case ActionKind::HILOptimize: return "hil-opt";
  case ActionKind::LSCGen: return "lscgen";
  case ActionKind::CodeGen: return "codegen";
  case ActionKind::Assemble: return "assemble";
  case ActionKind::Link: return "link";
  }
  return "unknown";
}
}
