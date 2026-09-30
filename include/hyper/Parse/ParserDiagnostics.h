#pragma once

#include "hyper/Parser.h"
#include <string>
#include <string_view>
#include <vector>

namespace hyper::parse {

struct ParserDiagnostic {
    enum class Severity {
        Note,
        Warning,
        Error,
        Fatal
    };

    Severity severity = Severity::Error;
    SourceLocation location{};
    std::string message;
    std::string fixIt;
};

class ParserDiagnosticEngine {
public:
    void note(SourceLocation location, std::string message);
    void warning(SourceLocation location, std::string message);
    void error(SourceLocation location, std::string message);
    void fatal(SourceLocation location, std::string message);
    void clear();
    bool hasError() const noexcept;
    const std::vector<ParserDiagnostic>& diagnostics() const noexcept;

private:
    std::vector<ParserDiagnostic> diagnostics_;
};

} // namespace hyper::parse
