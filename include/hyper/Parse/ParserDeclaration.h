#pragma once

#include <string>
#include <vector>

namespace hyper {

struct ParameterSyntax {
    std::string name;
    std::string type;
};

struct DeclarationSyntax {
    enum class Kind {
        Invalid,
        Function,
        Variable,
        Type,
        Import,
        Extension
    };

    Kind kind = Kind::Invalid;
    std::string name;
    std::vector<ParameterSyntax> parameters;
};

} // namespace hyper
