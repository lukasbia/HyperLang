#pragma once

#include <string>
#include <vector>

namespace hyper {

struct ParameterSyntax {
    std::string externalName;
    std::string localName;
    std::string type;
    std::string defaultValue;
    bool variadic = false;
    bool inout = false;
    bool ownershipSpecified = false;
    bool escaping = false;
};

struct DeclarationSyntax {
    enum class Kind {
        Invalid,
        Import,
        Function,
        Variable,
        Constant,
        Type,
        Struct,
        Class,
        Enum,
        Protocol,
        Extension,
        Operator,
        Macro,
        Initializer,
        Deinitializer,
        Subscript,
        Accessor
    };

    Kind kind = Kind::Invalid;
    std::string name;
    std::string qualifiedName;
    std::string returnType;
    std::string accessLevel;
    std::vector<ParameterSyntax> parameters;
    std::vector<std::string> modifiers;
    std::vector<std::string> attributes;
    std::vector<std::string> genericParameters;
    std::vector<std::string> inheritanceTypes;
};

class DeclarationParserSupport {
public:
    static bool isTypeDeclaration(
        DeclarationSyntax::Kind kind
    ) noexcept;

    static bool isCallableDeclaration(
        DeclarationSyntax::Kind kind
    ) noexcept;

    static bool isMemberDeclaration(
        DeclarationSyntax::Kind kind
    ) noexcept;

    static bool allowsGenericParameters(
        DeclarationSyntax::Kind kind
    ) noexcept;

    static bool requiresBody(
        DeclarationSyntax::Kind kind
    ) noexcept;
};

} // namespace hyper
