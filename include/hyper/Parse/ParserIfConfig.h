#pragma once

#include "hyper/Parser.h"
#include <memory>
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

class IfConfigParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseClause(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseCondition(Parser& parser);
    static bool startsDirective(const Parser& parser);
};

} // namespace hyper::parse
