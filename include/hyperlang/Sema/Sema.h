#pragma once
#include "hyperlang/AST/AST.h"
#include <string>
#include <vector>
namespace hyperlang::sema {
struct Diagnostic { std::string message; };
class Analyzer {
public: bool analyze(const ast::Program& program); const std::vector<Diagnostic>& diagnostics() const;
private: std::vector<Diagnostic> diagnostics_;
};
}
