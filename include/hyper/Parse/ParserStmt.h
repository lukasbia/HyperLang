#pragma once

#include "hyper/Parser.h"
#include <memory>
#include <string>
#include <vector>

namespace hyper::parse {

enum class StatementKind {
    Invalid, Expression, Declaration, Compound, If, While, For,
    Repeat, Switch, Return, Break, Continue, Defer, Throw, Do, Guard
};

struct StatementSyntax {
    StatementKind kind = StatementKind::Invalid;
    std::string label;
    std::vector<std::string> modifiers;
    std::vector<StatementSyntax> children;
};

class StatementParser {
public:
    static std::unique_ptr<SyntaxNode> parse(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseCompound(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseIf(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseWhile(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseFor(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseRepeat(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseSwitch(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseReturn(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseBreak(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseContinue(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseDefer(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseThrow(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseDo(Parser& parser);
    static std::unique_ptr<SyntaxNode> parseGuard(Parser& parser);
};

} // namespace hyper::parse
