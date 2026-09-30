#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct TypeSyntax {
    std::string name;
    std::vector<TypeSyntax> arguments;
    bool optional = false;
};

bool isBuiltinTypeName(const std::string& name) {
    return name == "Int" || name == "String" || name == "Bool" || name == "Float" || name == "Double" || name == "Void";
}

TypeSyntax makeType(const std::string& name) {
    TypeSyntax type;
    type.name = name;
    return type;
}

} // namespace hyper::parse
