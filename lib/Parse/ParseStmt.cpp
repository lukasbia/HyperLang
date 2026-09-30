#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

enum class StatementKind {
    Expression,
    Declaration,
    If,
    While,
    For,
    Return,
    Break,
    Continue,
    Defer,
    Do
};

struct StatementInfo {
    StatementKind kind = StatementKind::Expression;
    std::string spelling;
};

bool isStatementKeyword(const std::string& spelling) {
    return spelling == "if" || spelling == "while" || spelling == "for" || spelling == "return" || spelling == "break" || spelling == "continue" || spelling == "defer" || spelling == "do";
}

} // namespace hyper::parse
