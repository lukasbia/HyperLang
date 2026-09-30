#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct ParsedDeclaration {
    std::string name;
    std::vector<std::string> attributes;
};

ParsedDeclaration parseDeclarationName(const std::string& source) {
    ParsedDeclaration result;
    result.name = source;
    return result;
}

bool isDeclarationIntroducer(const std::string& token) {
    return token == "func" || token == "let" || token == "var" || token == "struct" || token == "enum" || token == "class" || token == "import";
}

} // namespace hyper::parse
