#pragma once

#include "hyper/Parse/ParserCore.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct ParserCheckpoint {
    ParserCursor cursor;
    ParseMode mode = ParseMode::SourceFile;
    std::size_t diagnosticCount = 0;
};

class PersistentParserState {
public:
    PersistentParserState() = default;

    ParserCheckpoint checkpoint() const;
    void restore(const ParserCheckpoint& checkpoint);

    void pushScope(std::string name);
    void popScope();
    const std::vector<std::string>& scopes() const;

    void setMode(ParseMode mode);
    ParseMode mode() const;

private:
    ParserCursor cursor_;
    ParseMode mode_ = ParseMode::SourceFile;
    std::vector<std::string> scopes_;
    std::size_t diagnosticCount_ = 0;
};

} // namespace hyper::parse
