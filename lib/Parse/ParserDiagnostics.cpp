#include "hyper/Parse/ParserDiagnostics.h"

namespace hyper {

void ParserDiagnostics::note(std::string message) {
    add({ParseDiagnostic::Severity::Note, {}, std::move(message)});
}

void ParserDiagnostics::warning(std::string message) {
    add({ParseDiagnostic::Severity::Warning, {}, std::move(message)});
}

void ParserDiagnostics::error(std::string message) {
    add({ParseDiagnostic::Severity::Error, {}, std::move(message)});
}

void ParserDiagnostics::fatal(std::string message) {
    add({ParseDiagnostic::Severity::Fatal, {}, std::move(message)});
}

void ParserDiagnostics::add(ParseDiagnostic diagnostic) {
    diagnostics_.push_back(std::move(diagnostic));
}

void ParserDiagnostics::clear() {
    diagnostics_.clear();
}

bool ParserDiagnostics::empty() const noexcept {
    return diagnostics_.empty();
}

bool ParserDiagnostics::hasErrors() const noexcept {
    for (const auto& diagnostic : diagnostics_) {
        if (diagnostic.severity == ParseDiagnostic::Severity::Error ||
            diagnostic.severity == ParseDiagnostic::Severity::Fatal) {
            return true;
        }
    }
    return false;
}

bool ParserDiagnostics::hasFatalErrors() const noexcept {
    for (const auto& diagnostic : diagnostics_) {
        if (diagnostic.severity == ParseDiagnostic::Severity::Fatal) {
            return true;
        }
    }
    return false;
}

std::size_t ParserDiagnostics::size() const noexcept {
    return diagnostics_.size();
}

const std::vector<ParseDiagnostic>& ParserDiagnostics::all() const noexcept {
    return diagnostics_;
}

std::vector<ParseDiagnostic>& ParserDiagnostics::all() noexcept {
    return diagnostics_;
}

} // namespace hyper
