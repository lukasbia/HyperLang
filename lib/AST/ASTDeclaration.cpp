#include <string>
#include <utility>
#include <vector>

namespace hyper::ast {

struct Parameter {
    std::string externalName;
    std::string localName;
    std::string typeName;
};

struct Declaration {
    std::string name;
    std::vector<Parameter> parameters;
};

struct FunctionDeclaration : Declaration {
    bool isAsync = false;
    bool isThrowing = false;
    bool isStatic = false;
    bool isPublic = false;
};

struct VariableDeclaration : Declaration {
    bool mutableValue = false;
    bool constantValue = false;
};

struct TypeDeclaration : Declaration {
    std::string underlyingType;
};

struct ImportDeclaration : Declaration {
    std::string module;
};

bool hasParameters(const Declaration& declaration) {
    return !declaration.parameters.empty();
}

bool isAsyncFunction(const FunctionDeclaration& declaration) {
    return declaration.isAsync;
}

bool isThrowingFunction(const FunctionDeclaration& declaration) {
    return declaration.isThrowing;
}

bool isMutable(const VariableDeclaration& declaration) {
    return declaration.mutableValue;
}

bool isConstant(const VariableDeclaration& declaration) {
    return declaration.constantValue;
}

} // namespace hyper::ast
