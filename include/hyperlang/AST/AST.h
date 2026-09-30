#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>
namespace hyperlang::ast {
enum class Ownership { Automatic, Owned, Borrowed, Shared, Weak };
struct Node { virtual ~Node() = default; };
struct Expression : Node { virtual ~Expression() = default; };
struct Statement : Node { virtual ~Statement() = default; };
struct Literal final : Expression { std::string value; explicit Literal(std::string v) : value(std::move(v)) {} };
struct Variable final : Expression { std::string name; Ownership ownership = Ownership::Automatic; explicit Variable(std::string n, Ownership o = Ownership::Automatic) : name(std::move(n)), ownership(o) {} };
struct Return final : Statement { std::unique_ptr<Expression> value; };
struct ExpressionStatement final : Statement { std::unique_ptr<Expression> expression; };
struct Function final : Node { std::string name; std::vector<std::string> parameters; std::vector<std::unique_ptr<Statement>> body; explicit Function(std::string n) : name(std::move(n)) {} };
struct Program final : Node { std::vector<std::unique_ptr<Function>> functions; };
} // namespace hyperlang::ast
