#pragma once
#include <string>
#include <vector>
namespace hyperlang::diagnostics {
enum class Severity { Note, Warning, Error, Fatal };
struct Diagnostic { Severity severity; std::string message; };
class Engine { public: void report(Severity severity, std::string message); const std::vector<Diagnostic>& diagnostics() const; bool hasErrors() const; private: std::vector<Diagnostic> diagnostics_; };
}
