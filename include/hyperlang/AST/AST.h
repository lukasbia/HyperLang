#pragma once
#include <memory>
#include <string>
#include <vector>
namespace hyperlang::ast {
struct Node { virtual ~Node()=default; };
struct Expression : Node { };
struct Statement : Node { };
struct Literal : Expression { std::string value; explicit Literal(std::string v):value(std::move(v)){} };
struct Variable : Expression { std::string name; explicit Variable(std::string n):name(std::move(n)){} };
struct Function : Node { std::string name; std::vector<std::unique_ptr<Statement>> body; explicit Function(std::string n):name(std::move(n)){} };
struct Program : Node { std::vector<std::unique_ptr<Function>> functions; };
}
