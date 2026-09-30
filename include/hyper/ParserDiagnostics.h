#pragma once

#include <string>
#include <vector>

namespace hyper {

struct ParserDiagnostic {
    std::string message;
    std::size_t position = 0;
};

class ParserDiagnostics {
public:
    void error(std::string message, std::size_t position = 0);
    bool empty() const;
    const std::vector<ParserDiagnostic>& diagnostics() const;

private:
    std::vector<ParserDiagnostic> diagnostics_;
};

} // namespace hyper
