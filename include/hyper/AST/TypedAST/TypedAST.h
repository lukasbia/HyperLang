#pragma once
#include <string>
#include <vector>
namespace hyper::typedast {
enum class NodeKind { Module, Function, Variable, Expression, Statement, TypeReference, Unknown };
enum class TypeKind { Void, Bool, Int, Float, Double, String, Array, Class, Protocol, Function, Unknown };
struct Type { TypeKind kind = TypeKind::Unknown; std::string name; bool optional = false; bool mutableType = false; };
struct Node { virtual ~Node() = default; NodeKind kind = NodeKind::Unknown; };
struct Expression : Node { Type type; bool lvalue = false; };
struct Statement : Node { bool terminating = false; bool reachable = true; };
struct Declaration : Node { std::string name; Type type; bool mutableValue = false; };
struct Function : Declaration { std::vector<Declaration> parameters; Type returnType; bool async = false; bool throwing = false; };
struct Module : Node { std::string name; std::vector<Node*> members; };
const char* nodeKindName(NodeKind kind) noexcept;
bool isNumeric(TypeKind kind) noexcept;
bool isReference(TypeKind kind) noexcept;
bool expressionHasType(const Expression& expression) noexcept;
bool expressionIsLValue(const Expression& expression) noexcept;
bool declarationHasType(const Declaration& declaration) noexcept;
bool declarationIsMutable(const Declaration& declaration) noexcept;
bool statementIsTerminating(const Statement& statement) noexcept;
bool statementIsReachable(const Statement& statement) noexcept;
}
