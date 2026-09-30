#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
namespace hyper::sema {
enum class DiagnosticSeverity { Note, Warning, Error };
struct Diagnostic { DiagnosticSeverity severity = DiagnosticSeverity::Error; std::string message; };
struct TypeInfo { std::string name; bool valid = false; };
struct Symbol { std::string name; TypeInfo type; bool mutableValue = false; };
class Scope { public: bool declare(Symbol symbol); const Symbol* lookup(std::string_view name) const; private: std::unordered_map<std::string, Symbol> symbols_; };
class Sema { public: void enterScope(); void leaveScope(); bool declare(Symbol symbol); const Symbol* lookup(std::string_view name) const; bool requireSymbol(std::string_view name); const std::vector<Diagnostic>& diagnostics() const noexcept; void clearDiagnostics(); private: std::vector<Scope> scopes_; std::vector<Diagnostic> diagnostics_; };
} // namespace hyper::sema
