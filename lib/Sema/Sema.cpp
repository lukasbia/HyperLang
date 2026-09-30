#include "hyperlang/Sema/Sema.h"
#include <unordered_set>
namespace hyperlang::sema {
bool Analyzer::analyze(const ast::Program& program) {
    diagnostics_.clear();
    std::unordered_set<std::string> names;
    for (const auto& function : program.functions) {
        if (!function || function->name.empty()) {
            diagnostics_.push_back({"function name cannot be empty"});
            continue;
        }
        if (!names.insert(function->name).second)
            diagnostics_.push_back({"duplicate function: " + function->name});
        std::unordered_set<std::string> parameters;
        for (const auto& parameter : function->parameters) {
            if (!parameters.insert(parameter).second)
                diagnostics_.push_back({"duplicate parameter '" + parameter + "' in function '" + function->name + "'"});
        }
    }
    return diagnostics_.empty();
}
const std::vector<Diagnostic>& Analyzer::diagnostics() const { return diagnostics_; }
} // namespace hyperlang::sema
