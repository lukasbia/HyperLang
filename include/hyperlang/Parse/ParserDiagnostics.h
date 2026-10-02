#ifndef HYPERLANG_PARSE_PARSER_DIAGNOSTICS_H
#define HYPERLANG_PARSE_PARSER_DIAGNOSTICS_H
#include "hyperlang/Parse/LexerDiagnostics.h"
#include <string_view>
namespace hyperlang {
class ParserDiagnostics {
public:
  explicit ParserDiagnostics(DiagnosticList &d) : diagnostics_(d) {}
  void error(SourceLocation l, std::string_view m);
  void warning(SourceLocation l, std::string_view m);
  void note(SourceLocation l, std::string_view m);
private:
  DiagnosticList &diagnostics_;
};
}
#endif
