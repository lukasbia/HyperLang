#pragma once

#include <string>
#include <vector>
#include "hyper/AST/AST.h"

namespace hyper::ast {

struct Parameter {
    std::string externalName;
    std::string localName;
    std::string typeName;
};

class Declaration : public Node {
public:
    std::string name;
    std::vector<Parameter> parameters;
};

class FunctionDeclaration final : public Declaration {
public:
    bool isAsync = false;
    bool isThrowing = false;
    bool isStatic = false;
    bool isPublic = false;
};

class VariableDeclaration final : public Declaration {
public:
    bool mutableValue = false;
    bool constantValue = false;
};

class TypeDeclaration final : public Declaration {
public:
    std::string underlyingType;
};

class ImportDeclaration final : public Declaration {
public:
    std::string module;
};

} // namespace hyper::ast
