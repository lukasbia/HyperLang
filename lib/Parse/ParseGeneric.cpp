#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct GenericParameter {
    std::string name;
    std::vector<std::string> constraints;
};

bool isGenericParameterName(const std::string& name) {
    return !name.empty() && name.front() != ':';
}

GenericParameter makeGenericParameter(const std::string& name) {
    GenericParameter parameter;
    parameter.name = name;
    return parameter;
}

} // namespace hyper::parse
