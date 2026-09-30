#pragma once

#include <string>
#include <vector>

namespace hyper::parse {

enum class StatementKind {
    Invalid,
    Expression,
    Declaration,
    Compound,
    If,
    While,
    For,
    Repeat,
    Switch,
    Return,
    Break,
    Continue,
    Defer,
    Throw,
    Do,
    Guard
};

struct StatementSyntax {
    StatementKind kind = StatementKind::Invalid;
    std::string label;
    std::vector<std::string> modifiers;
    std::vector<StatementSyntax> children;
};

} // namespace hyper::parse
