#pragma once

#include "hyper/Parse/SourceLocation.h"

#include <cstddef>
#include <string>
#include <vector>

namespace hyper {

struct ParseDiagnostic {
    enum class Severity { Note, Warning, Error, Fatal };

    Severity severity = Severity::Error;
    SourceLocation location;
    std::string message;
    std::string fixIt;
    std::string category;
    std::vector<std::string> notes;
    std::vector<std::string> highlights;
};

class ParserDiagnostics {
public:
    void note(std::string message);
    void warning(std::string message);
    void error(std::string message);
    void fatal(std::string message);
    void add(ParseDiagnostic diagnostic);
    void clear();

    bool empty() const noexcept;
    bool hasErrors() const noexcept;
    bool hasFatalErrors() const noexcept;
    std::size_t size() const noexcept;

    const std::vector<ParseDiagnostic>& all() const noexcept;
    std::vector<ParseDiagnostic>& all() noexcept;

private:
    std::vector<ParseDiagnostic> diagnostics_;
};

} // namespace hyper
