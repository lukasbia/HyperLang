#pragma once

#include <cstddef>
#include <string>

namespace hyper {

struct SourceLocation {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
    std::string file;
};

struct SourceRange {
    SourceLocation begin;
    SourceLocation end;
};

} // namespace hyper
