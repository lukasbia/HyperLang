#include <string>
#include <vector>

namespace hyper::ast {

struct ImportPath {
    std::vector<std::string> components;

    bool empty() const noexcept {
        return components.empty();
    }
};

struct ImportDeclaration {
    ImportPath path;
    std::string alias;
    bool isExported = false;
};

bool hasAlias(const ImportDeclaration& declaration) {
    return !declaration.alias.empty();
}

bool isExportedImport(const ImportDeclaration& declaration) {
    return declaration.isExported;
}

} // namespace hyper::ast
