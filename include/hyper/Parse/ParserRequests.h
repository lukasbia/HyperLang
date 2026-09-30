#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

struct ParserRequest {
    enum class Kind {
        None,
        Completion,
        SyntaxHighlighting,
        CodeStructure,
        Diagnostics,
        Format,
        Documentation
    };

    Kind kind = Kind::None;
    std::size_t offset = 0;
    std::string payload;
    std::vector<std::string> options;
};

} // namespace hyper::parse
