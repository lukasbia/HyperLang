//===--- ASTDump.h - HyperLang AST Debug Dump ----------------------------===//
#ifndef HYPERLANG_PARSE_AST_DUMP_H
#define HYPERLANG_PARSE_AST_DUMP_H
#include "hyperlang/Parse/AST.h"
#include <ostream>
namespace hyperlang::ast {
void dump(const Node &node, std::ostream &output);
}
#endif
