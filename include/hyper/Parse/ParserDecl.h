#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

struct ParameterSyntax {
    std::string externalName;
    std::string localName;
    std::string typeName;
    bool variadic = false;
    bool hasDefaultValue = false;
};

struct DeclarationSyntax {
    enum class Kind {
        Invalid,
        Import,
        Function,
        Variable,
        Constant,
        TypeAlias,
        Struct,
        Enum,
        Protocol,
        Extension,
        Operator,
        Macro
    };

    Kind kind = Kind::Invalid;
    std::string name;
    std::string accessLevel;
    std::vector<ParameterSyntax> parameters;
    std::vector<std::string> attributes;
    std::vector<std::string> genericParameters;
};

} // namespace hyper::parse
