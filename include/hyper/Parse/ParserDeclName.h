#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <string_view>

namespace hyper::parse {

struct DeclarationNameSyntax {
    std::string baseName;
    std::string argumentLabel;
    std::string localName;
    bool isOperator = false;
    bool isEscaped = false;
};

class DeclarationNameParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static DeclarationNameSyntax parseText(std::string_view text);
    static bool isOperatorName(std::string_view text) noexcept;
    static bool isValidIdentifier(std::string_view text) noexcept;
};

} // namespace hyper::parse
