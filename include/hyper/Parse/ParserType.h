#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

enum class TypeKind {
    Invalid,
    Named,
    Tuple,
    Function,
    Array,
    Dictionary,
    Optional,
    Metatype,
    Existential,
    Opaque,
    GenericParameter
};

struct TypeSyntax {
    TypeKind kind = TypeKind::Invalid;
    std::string name;
    std::vector<TypeSyntax> arguments;
    bool isInout = false;
    bool isEscaping = false;
};

} // namespace hyper::parse
