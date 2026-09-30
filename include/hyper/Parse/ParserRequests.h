#pragma once

#include "hyper/Parser.h"
#include <cstddef>
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
        Documentation,
        Outline,
        SelectionRange
    };

    Kind kind = Kind::None;
    std::size_t offset = 0;
    std::string payload;
    std::vector<std::string> options;
};

class ParserRequestProcessor {
public:
    static ParserRequest::Kind classify(std::string_view name);
    static bool supports(ParserRequest::Kind kind) noexcept;
    static void normalize(ParserRequest& request);
};

} // namespace hyper::parse
