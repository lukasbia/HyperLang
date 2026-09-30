#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

struct IfConfigClauseSyntax {
    std::string condition;
    bool active = true;
    std::vector<std::string> sourceTokens;
};

struct IfConfigSyntax {
    std::vector<IfConfigClauseSyntax> clauses;
    bool hasElseClause = false;
};

} // namespace hyper::parse
