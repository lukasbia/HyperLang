#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct IfConfigClause {
    std::string condition;
    std::vector<std::string> tokens;
};

bool isIfConfigDirective(const std::string& token) {
    return token == "#if" || token == "#elseif" || token == "#else" || token == "#endif";
}

bool isTerminalIfConfigDirective(const std::string& token) {
    return token == "#endif";
}

} // namespace hyper::parse
