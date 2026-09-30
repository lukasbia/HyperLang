#include <string>
#include <vector>

namespace hyper::ast {

struct GenericParameter {
    std::string name;
    std::vector<std::string> constraints;
};

struct GenericParameterClause {
    std::vector<GenericParameter> parameters;
};

struct GenericRequirement {
    std::string left;
    std::string relation;
    std::string right;
};

struct GenericWhereClause {
    std::vector<GenericRequirement> requirements;
};

bool hasConstraints(const GenericParameter& parameter) {
    return !parameter.constraints.empty();
}

bool hasRequirements(const GenericWhereClause& clause) {
    return !clause.requirements.empty();
}

} // namespace hyper::ast
