#include <string>
#include <vector>

namespace hyper::ast {

struct MacroArgument {
    std::string name;
    std::string value;
};

struct MacroExpansion {
    std::string name;
    std::vector<MacroArgument> arguments;
};

struct MacroDeclaration {
    std::string name;
    std::vector<std::string> parameters;
    std::string body;
};

bool hasArguments(const MacroExpansion& expansion) {
    return !expansion.arguments.empty();
}

bool hasParameters(const MacroDeclaration& declaration) {
    return !declaration.parameters.empty();
}

} // namespace hyper::ast
