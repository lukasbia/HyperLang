#include <string>
#include <vector>

namespace hyper::ast {

struct ExtensionDeclaration {
    std::string extendedType;
    std::vector<std::string> inheritedTypes;
    std::vector<std::string> members;
};

bool hasMembers(const ExtensionDeclaration& declaration) {
    return !declaration.members.empty();
}

bool hasConformances(const ExtensionDeclaration& declaration) {
    return !declaration.inheritedTypes.empty();
}

} // namespace hyper::ast
