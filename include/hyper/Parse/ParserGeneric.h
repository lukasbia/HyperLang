#pragma once

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

} // namespace hyper::parse
