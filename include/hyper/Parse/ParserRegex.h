#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

struct RegexLiteralSyntax {
    std::string pattern;
    std::string options;
    std::vector<std::string> captures;
    bool multiline = false;
    bool caseInsensitive = false;
};

} // namespace hyper::parse
