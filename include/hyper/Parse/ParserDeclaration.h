#pragma once

#include <string>
#include <vector>

namespace hyper {

struct ParameterSyntax {
    std::string externalName;
    std::string localName;
    std::string type;
    bool variadic = false;
    bool inout = false;
    bool ownershipSpecified = false;
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
        Macro
    };

    Kind kind = Kind::Invalid;
    std::string name;
    std::string qualifiedName;
    std::string returnType;
    std::vector<ParameterSyntax> parameters;
    std::vector<std::string> modifiers;
    std::vector<std::string> genericParameters;
};

class DeclarationParserSupport {
public:
    static bool isTypeDeclaration(DeclarationSyntax::Kind kind) noexcept;
    static bool isCallableDeclaration(DeclarationSyntax::Kind kind) noexcept;
    static bool isMemberDeclaration(DeclarationSyntax::Kind kind) noexcept;
    static bool allowsGenericParameters(DeclarationSyntax::Kind kind) noexcept;
};

} // namespace hyper
