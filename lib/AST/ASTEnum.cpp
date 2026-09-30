#include <string>
#include <vector>

namespace hyper::ast {

struct EnumElement {
    std::string name;
    std::vector<std::string> associatedTypes;
};

struct EnumDeclaration {
    std::string name;
    std::vector<EnumElement> elements;
    bool isIndirect = false;
};

bool hasCases(const EnumDeclaration& declaration) {
    return !declaration.elements.empty();
}

bool isIndirect(const EnumDeclaration& declaration) {
    return declaration.isIndirect;
}

} // namespace hyper::ast
