#include <string>
#include <vector>

namespace hyper::ast {

struct Modifier {
    std::string name;
    bool isAccess = false;
    bool isDeclaration = false;
    bool isOwnership = false;
    bool isConcurrency = false;
};

bool isAccessModifier(const Modifier& modifier) {
    return modifier.isAccess;
}

bool isOwnershipModifier(const Modifier& modifier) {
    return modifier.isOwnership;
}

bool isConcurrencyModifier(const Modifier& modifier) {
    return modifier.isConcurrency;
}

} // namespace hyper::ast
