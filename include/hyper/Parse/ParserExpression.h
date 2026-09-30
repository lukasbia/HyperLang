#pragma once

#include <string>
#include <vector>

namespace hyper {

struct ExpressionNode {
    enum class Kind {
        Invalid,
        Identifier,
        Literal,
        Unary,
        Binary,
        Call,
        Member,
        Group
    };

    Kind kind = Kind::Invalid;
    std::string text;
    std::vector<ExpressionNode> children;
};

} // namespace hyper
