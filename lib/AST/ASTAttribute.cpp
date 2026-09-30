#include <string>
#include <utility>
#include <vector>

namespace hyper::ast {

struct AttributeArgument {
    std::string label;
    std::string value;
};

struct Attribute {
    std::string name;
    std::vector<AttributeArgument> arguments;
    bool hasArguments() const noexcept {
        return !arguments.empty();
    }
};

bool isBuiltinAttribute(const Attribute& attribute) {
    return attribute.name == "available" ||
           attribute.name == "deprecated" ||
           attribute.name == "discardableResult";
}

bool isSourceAttribute(const Attribute& attribute) {
    return attribute.name.rfind("@", 0) == 0;
}

} // namespace hyper::ast
