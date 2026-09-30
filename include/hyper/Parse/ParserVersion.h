#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>

namespace hyper::parse {

struct VersionComponent {
    unsigned value = 0;
    bool wildcard = false;
};

struct VersionConstraintSyntax {
    enum class Operator {
        Equal, NotEqual, Less, LessEqual, Greater, GreaterEqual, Compatible
    };

    Operator operation = Operator::Equal;
    VersionComponent major;
    VersionComponent minor;
    VersionComponent patch;
    std::string originalText;
};

class VersionParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static VersionConstraintSyntax parseConstraint(std::string_view text);
    static VersionComponent parseComponent(std::string_view text);
    static bool isVersionStart(const Parser& parser);
};

} // namespace hyper::parse
