#include <string>
#include <vector>

namespace hyper::ast {

struct ClassMember {
    std::string name;
    std::string type;
    bool isStatic = false;
};

struct ClassDeclaration {
    std::string name;
    std::vector<ClassMember> members;
    std::vector<std::string> inheritedTypes;
};

bool hasMembers(const ClassDeclaration& declaration) {
    return !declaration.members.empty();
}

bool hasInheritance(const ClassDeclaration& declaration) {
    return !declaration.inheritedTypes.empty();
}

} // namespace hyper::ast
