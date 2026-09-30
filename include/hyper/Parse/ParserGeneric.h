#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

struct GenericParameterSyntax {
    std::string name;
    std::vector<std::string> inheritedConstraints;
    bool variadic = false;
};

struct GenericRequirementSyntax {
    std::string left;
    std::string relation;
    std::string right;
};

struct GenericClauseSyntax {
    std::vector<GenericParameterSyntax> parameters;
    std::vector<GenericRequirementSyntax> requirements;
};

class GenericParser {
public:
    static std::unique_ptr<SyntaxNode> parseClause(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseParameter(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseRequirement(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseArgumentList(Parser& parser);
    static bool startsGenericClause(const Parser& parser);
};

} // namespace hyper::parse
