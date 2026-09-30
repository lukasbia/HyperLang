#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct ExpressionInfo {
    std::string spelling;
    int precedence = -1;
};

int expressionPrecedence(const std::string& operatorSpelling) {
    if (operatorSpelling == "||") return 10;
    if (operatorSpelling == "&&") return 20;
    if (operatorSpelling == "==" || operatorSpelling == "!=") return 30;
    if (operatorSpelling == "<" || operatorSpelling == ">" || operatorSpelling == "<=" || operatorSpelling == ">=") return 40;
    if (operatorSpelling == "+" || operatorSpelling == "-") return 50;
    if (operatorSpelling == "*" || operatorSpelling == "/" || operatorSpelling == "%") return 60;
    return -1;
}

bool isBinaryOperator(const std::string& spelling) {
    return expressionPrecedence(spelling) >= 0;
}

} // namespace hyper::parse
