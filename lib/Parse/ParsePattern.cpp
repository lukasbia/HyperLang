#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct PatternNode {
    enum class Kind { Identifier, Wildcard, Literal, Tuple, EnumCase };
    Kind kind = Kind::Identifier;
    std::string text;
    std::vector<PatternNode> children;
};

PatternNode makeWildcardPattern() {
    PatternNode pattern;
    pattern.kind = PatternNode::Kind::Wildcard;
    pattern.text = "_";
    return pattern;
}

bool isWildcardPattern(const std::string& spelling) {
    return spelling == "_";
}

} // namespace hyper::parse
