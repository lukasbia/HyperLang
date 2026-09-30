#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace hyper {

struct SourceLocation;

struct ParseDiagnostic {
    enum class Severity {
        Note,
        Warning,
        Error
    };

    Severity severity = Severity::Error;
    SourceLocation* location = nullptr;
    std::string message;
    std::string fixIt;
    std::vector<std::string> notes;
};

class ParserDiagnostics {
public:
    void note(std::string message);
    void warning(std::string message);
    void error(std::string message);

    void add(ParseDiagnostic diagnostic);
    void clear();

    bool empty() const noexcept;
    bool hasErrors() const noexcept;
    std::size_t size() const noexcept;

    const std::vector<ParseDiagnostic>& all() const noexcept;
    std::vector<ParseDiagnostic>& all() noexcept;

private:
    std::vector<ParseDiagnostic> diagnostics_;
};

} // namespace hyper
