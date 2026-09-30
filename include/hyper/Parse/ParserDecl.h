#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

struct ParameterSyntax {
    std::string externalName;
    std::string localName;
    std::string typeName;
    std::string defaultValue;
    bool variadic = false;
    bool inout = false;
    bool escaping = false;
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
    std::string modifiers;
    std::vector<ParameterSyntax> parameters;
    std::vector<std::string> attributes;
    std::vector<std::string> genericParameters;
    std::vector<std::string> inheritedTypes;
};

class DeclarationParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseImport(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseFunction(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseVariable(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseType(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseStruct(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseClass(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseEnum(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseProtocol(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseExtension(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseParameters(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseAttributes(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseModifiers(Parser& parser);
};

} // namespace hyper::parse
