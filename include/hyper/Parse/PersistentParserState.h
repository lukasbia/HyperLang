#pragma once

#include "hyper/Parse/ParserCore.h"
#include <cstddef>
#include <string>
#include <vector>

namespace hyper::parse {

struct ParserCheckpoint {
    ParserCursor cursor;
    ParseMode mode = ParseMode::SourceFile;
    std::size_t diagnosticCount = 0;
    std::size_t scopeDepth = 0;
};

class PersistentParserState {
public:
    PersistentParserState() = default;

    ParserCheckpoint checkpoint() const;
    void restore(const ParserCheckpoint& checkpoint);
    void reset();

    void pushScope(std::string name);
    void popScope();
    const std::vector<std::string>& scopes() const;
    bool hasScope(std::string_view name) const;

    void setMode(ParseMode mode);
    ParseMode mode() const;

    void setCursor(ParserCursor cursor);
    ParserCursor cursor() const;

    void setDiagnosticCount(std::size_t count);
    std::size_t diagnosticCount() const;

private:
    ParserCursor cursor_;
    ParseMode mode_ = ParseMode::SourceFile;
    std::vector<std::string> scopes_;
    std::size_t diagnosticCount_ = 0;
};

} // namespace hyper::parse
