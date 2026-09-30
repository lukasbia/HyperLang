#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

enum class TypeKind {
    Invalid, Named, Tuple, Function, Array, Dictionary, Optional,
    Metatype, Existential, Opaque, GenericParameter, Inout, Composition
};

struct TypeSyntax {
    TypeKind kind = TypeKind::Invalid;
    std::string name;
    std::vector<TypeSyntax> arguments;
    bool isInout = false;
    bool isEscaping = false;
};

class TypeParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseNamed(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseTuple(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseFunction(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseArray(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseDictionary(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseOptional(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseGeneric(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseComposition(Parser& parser);
};

} // namespace hyper::parse
