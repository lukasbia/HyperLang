#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

struct RegexLiteralSyntax {
    std::string pattern;
    std::string options;
    std::vector<std::string> captures;
    bool multiline = false;
    bool caseInsensitive = false;
    bool dotAll = false;
    bool unicode = true;
};

class RegexParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseLiteral(Parser& parser);
    static std::unique_ptr<SyntaxNode> parsePattern(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseOptions(Parser& parser);
    static std::vector<std::string> collectCaptures(std::string_view pattern);
};

} // namespace hyper::parse
