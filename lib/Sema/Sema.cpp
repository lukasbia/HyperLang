#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hyper::sema {

struct Diagnostic {
    enum class Severity { Note, Warning, Error };
    Severity severity = Severity::Error;
    std::string message;
};

struct TypeInfo {
    std::string name;
    bool valid = false;
};

struct Symbol {
    std::string name;
    TypeInfo type;
    bool mutableValue = false;
};

class Scope {
public:
    bool declare(Symbol symbol) {
        return symbols_.emplace(symbol.name, std::move(symbol)).second;
    }

    const Symbol* lookup(std::string_view name) const {
        auto it = symbols_.find(std::string(name));
        if (it == symbols_.end()) {
            return nullptr;
        }
        return &it->second;
    }

private:
    std::unordered_map<std::string, Symbol> symbols_;
};

class Sema {
public:
    void enterScope() {
        scopes_.emplace_back();
    }

    void leaveScope() {
        if (!scopes_.empty()) {
            scopes_.pop_back();
        }
    }

    bool declare(Symbol symbol) {
        if (scopes_.empty()) {
            enterScope();
        }
        if (!scopes_.back().declare(symbol)) {
            diagnostics_.push_back({
                Diagnostic::Severity::Error,
                "redeclaration of '" + symbol.name + "'"
            });
            return false;
        }
        return true;
    }

    const Symbol* lookup(std::string_view name) const {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
            if (const Symbol* symbol = it->lookup(name)) {
                return symbol;
            }
        }
        return nullptr;
    }

    bool requireSymbol(std::string_view name) {
        if (lookup(name) != nullptr) {
            return true;
        }
        diagnostics_.push_back({
            Diagnostic::Severity::Error,
            "use of unresolved identifier '" + std::string(name) + "'"
        });
        return false;
    }

    const std::vector<Diagnostic>& diagnostics() const noexcept {
        return diagnostics_;
    }

private:
    std::vector<Scope> scopes_;
    std::vector<Diagnostic> diagnostics_;
};

} // namespace hyper::sema
