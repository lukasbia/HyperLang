#include <string>
#include <vector>

namespace hyper::ast {

struct ProtocolRequirement {
    std::string name;
    std::string signature;
};

struct ProtocolDeclaration {
    std::string name;
    std::vector<ProtocolRequirement> requirements;
    std::vector<std::string> inheritedProtocols;
};

bool hasRequirements(const ProtocolDeclaration& declaration) {
    return !declaration.requirements.empty();
}

bool hasInheritedProtocols(const ProtocolDeclaration& declaration) {
    return !declaration.inheritedProtocols.empty();
}

} // namespace hyper::ast
