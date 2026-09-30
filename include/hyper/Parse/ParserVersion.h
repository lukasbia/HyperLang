#pragma once

#include <string>

namespace hyper::parse {

struct VersionComponent {
    unsigned value = 0;
    bool wildcard = false;
};

struct VersionConstraintSyntax {
    enum class Operator {
        Equal,
        NotEqual,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Compatible
    };

    Operator operation = Operator::Equal;
    VersionComponent major;
    VersionComponent minor;
    VersionComponent patch;
    std::string originalText;
};

} // namespace hyper::parse
