#include "hyper/Sema/Sema.h"

#include <utility>

namespace hyper::sema {

bool Scope::declare(Symbol symbol) {
    return symbols_.emplace(symbol.name, std::move(symbol)).second;
}

const Symbol* Scope::lookup(std::string_view name) const {
    const auto it = symbols_.find(std::string(name));
    if (it == symbols_.end()) {
        return nullptr;
    }
    return &it->second;
}

void Sema::enterScope() {
    scopes_.emplace_back();
}

void Sema::leaveScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
    }
}

bool Sema::declare(Symbol symbol) {
    if (scopes_.empty()) {
        enterScope();
    }

    if (!scopes_.back().declare(symbol)) {
        diagnostics_.push_back({
            DiagnosticSeverity::Error,
            "redeclaration of '" + symbol.name + "'"
        });
        return false;
    }

    return true;
}

const Symbol* Sema::lookup(std::string_view name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (const Symbol* symbol = it->lookup(name)) {
            return symbol;
        }
    }

    return nullptr;
}

bool Sema::requireSymbol(std::string_view name) {
    if (lookup(name) != nullptr) {
        return true;
    }

    diagnostics_.push_back({
        DiagnosticSeverity::Error,
        "use of unresolved identifier '" + std::string(name) + "'"
    });
    return false;
}

const std::vector<Diagnostic>& Sema::diagnostics() const noexcept {
    return diagnostics_;
}

void Sema::clearDiagnostics() {
    diagnostics_.clear();
}

} // namespace hyper::sema
