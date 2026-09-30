#include <string>
#include <vector>

namespace hyper::ast {

struct Field {
    std::string name;
    std::string type;
    bool mutableValue = false;
};

struct StructDeclaration {
    std::string name;
    std::vector<Field> fields;
    bool isGeneric = false;
};

bool hasFields(const StructDeclaration& declaration) {
    return !declaration.fields.empty();
}

bool hasGenericParameters(const StructDeclaration& declaration) {
    return declaration.isGeneric;
}

} // namespace hyper::ast
