//===--- ParserResult.h - HyperLang Parser Result -------------------------===//
#ifndef HYPERLANG_PARSE_PARSER_RESULT_H
#define HYPERLANG_PARSE_PARSER_RESULT_H
#include "hyperlang/Parse/AST.h"
#include "hyperlang/Parse/LexerDiagnostics.h"
#include <memory>
namespace hyperlang {
struct ParserResult {
  std::unique_ptr<ast::SourceFile> sourceFile;
  DiagnosticList diagnostics;
  bool hasErrors() const {
    for (const auto &d : diagnostics)
      if (d.severity == Diagnostic::Severity::Error)
        return true;
    return false;
  }
  explicit operator bool() const { return sourceFile != nullptr && !hasErrors(); }
};
}
#endif
