#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

enum class PatternKind {
    Invalid,
    Wildcard,
    Identifier,
    Literal,
    Tuple,
    EnumCase,
    Optional,
    TypeCast,
    ValueBinding,
    Expression
};

struct PatternSyntax {
    PatternKind kind = PatternKind::Invalid;
    std::string text;
    std::vector<PatternSyntax> children;
};

} // namespace hyper::parse
