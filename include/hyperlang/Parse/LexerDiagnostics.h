//===--- LexerDiagnostics.h - HyperLang Lexer Diagnostics ----------------===//

#ifndef HYPERLANG_PARSE_LEXER_DIAGNOSTICS_H
#define HYPERLANG_PARSE_LEXER_DIAGNOSTICS_H

#include <cstddef>
#include <string>
#include <vector>

namespace hyperlang {

struct SourceLocation {
  std::size_t offset = 0;
  std::size_t line = 1;
  std::size_t column = 1;
};

struct SourceRange {
  SourceLocation start;
  SourceLocation end;
};

struct Diagnostic {
  enum class Severity { Note, Warning, Error };

  Severity severity = Severity::Error;
  SourceLocation location{};
  std::string message;
};

using DiagnosticList = std::vector<Diagnostic>;

} // namespace hyperlang

#endif
