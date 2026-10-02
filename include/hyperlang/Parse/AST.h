//===--- AST.h - HyperLang Abstract Syntax Tree ---------------------------===//

#ifndef HYPERLANG_PARSE_AST_H
#define HYPERLANG_PARSE_AST_H

#include "hyperlang/Parse/LexerDiagnostics.h"
#include "hyperlang/Parse/TokenKinds.h"
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace hyperlang::ast {

enum class NodeKind : std::uint16_t {
  SourceFile, Attribute, ImportDeclaration, VariableDeclaration,
  FunctionDeclaration, ParameterDeclaration, ReturnStatement, IfStatement,
  WhileStatement, ForStatement, GuardStatement, DeferStatement,
  SwitchStatement, CaseClause, BreakStatement, ContinueStatement,
  ExpressionStatement, BlockStatement, BinaryExpression, UnaryExpression,
  AssignmentExpression, CallExpression, MemberExpression, SubscriptExpression,
  RangeExpression, IdentifierExpression, IntegerLiteralExpression,
  FloatingLiteralExpression, StringLiteralExpression, BooleanLiteralExpression,
  NilLiteralExpression, ArrayExpression, TupleExpression, TypeName, Unknown
};

struct Node {
  NodeKind kind = NodeKind::Unknown;
  SourceRange range{};
  explicit Node(NodeKind k, SourceRange r = {}) : kind(k), range(r) {}
  virtual ~Node() = default;
};
using NodePtr = std::unique_ptr<Node>;

struct TypeNode : Node {
  std::string name;
  bool optional = false;
  std::vector<std::unique_ptr<TypeNode>> arguments;
  TypeNode(std::string n, SourceRange r = {}) : Node(NodeKind::TypeName, r), name(std::move(n)) {}
};

struct Expression : Node { using Node::Node; };
struct Statement : Node { using Node::Node; };
using ExpressionPtr = std::unique_ptr<Expression>;
using StatementPtr = std::unique_ptr<Statement>;

struct Attribute : Node {
  std::string spelling;
  std::vector<ExpressionPtr> arguments;
  Attribute(std::string s, SourceRange r = {}) : Node(NodeKind::Attribute, r), spelling(std::move(s)) {}
};

struct ParameterDeclaration : Node {
  std::string name;
  std::unique_ptr<TypeNode> type;
  ExpressionPtr defaultValue;
  ParameterDeclaration(std::string n, SourceRange r = {}) : Node(NodeKind::ParameterDeclaration, r), name(std::move(n)) {}
};

struct BlockStatement : Statement {
  std::vector<StatementPtr> statements;
  explicit BlockStatement(SourceRange r = {}) : Statement(NodeKind::BlockStatement, r) {}
};

struct ImportDeclaration : Statement {
  std::string moduleName;
  ImportDeclaration(std::string n, SourceRange r = {}) : Statement(NodeKind::ImportDeclaration, r), moduleName(std::move(n)) {}
};

struct VariableDeclaration : Statement {
  bool isMutable = false;
  bool isConstant = false;
  std::string name;
  std::unique_ptr<TypeNode> type;
  ExpressionPtr initializer;
  VariableDeclaration(std::string n, SourceRange r = {}) : Statement(NodeKind::VariableDeclaration, r), name(std::move(n)) {}
};

struct FunctionDeclaration : Statement {
  std::string name;
  bool isAsync = false;
  bool isThrowing = false;
  std::vector<std::unique_ptr<ParameterDeclaration>> parameters;
  std::unique_ptr<TypeNode> returnType;
  std::unique_ptr<BlockStatement> body;
  std::vector<std::unique_ptr<Attribute>> attributes;
  FunctionDeclaration(std::string n, SourceRange r = {}) : Statement(NodeKind::FunctionDeclaration, r), name(std::move(n)) {}
};

struct ReturnStatement : Statement {
  ExpressionPtr value;
  explicit ReturnStatement(SourceRange r = {}) : Statement(NodeKind::ReturnStatement, r) {}
};

struct IfStatement : Statement {
  ExpressionPtr condition;
  std::unique_ptr<BlockStatement> thenBody;
  std::unique_ptr<BlockStatement> elseBody;
  explicit IfStatement(SourceRange r = {}) : Statement(NodeKind::IfStatement, r) {}
};

struct WhileStatement : Statement {
  ExpressionPtr condition;
  std::unique_ptr<BlockStatement> body;
  explicit WhileStatement(SourceRange r = {}) : Statement(NodeKind::WhileStatement, r) {}
};

struct ForStatement : Statement {
  std::string pattern;
  ExpressionPtr sequence;
  std::unique_ptr<BlockStatement> body;
  explicit ForStatement(SourceRange r = {}) : Statement(NodeKind::ForStatement, r) {}
};

struct GuardStatement : Statement {
  ExpressionPtr condition;
  std::unique_ptr<BlockStatement> elseBody;
  explicit GuardStatement(SourceRange r = {}) : Statement(NodeKind::GuardStatement, r) {}
};

struct DeferStatement : Statement {
  std::unique_ptr<BlockStatement> body;
  explicit DeferStatement(SourceRange r = {}) : Statement(NodeKind::DeferStatement, r) {}
};

struct CaseClause : Node {
  std::vector<ExpressionPtr> patterns;
  std::unique_ptr<BlockStatement> body;
  explicit CaseClause(SourceRange r = {}) : Node(NodeKind::CaseClause, r) {}
};

struct SwitchStatement : Statement {
  ExpressionPtr subject;
  std::vector<std::unique_ptr<CaseClause>> cases;
  explicit SwitchStatement(SourceRange r = {}) : Statement(NodeKind::SwitchStatement, r) {}
};

struct BreakStatement : Statement {
  explicit BreakStatement(SourceRange r = {}) : Statement(NodeKind::BreakStatement, r) {}
};

struct ContinueStatement : Statement {
  explicit ContinueStatement(SourceRange r = {}) : Statement(NodeKind::ContinueStatement, r) {}
};

struct ExpressionStatement : Statement {
  ExpressionPtr expression;
  explicit ExpressionStatement(ExpressionPtr e, SourceRange r = {}) : Statement(NodeKind::ExpressionStatement, r), expression(std::move(e)) {}
};

struct IdentifierExpression : Expression {
  std::string name;
  IdentifierExpression(std::string n, SourceRange r = {}) : Expression(NodeKind::IdentifierExpression, r), name(std::move(n)) {}
};

struct IntegerLiteralExpression : Expression {
  std::uint64_t value = 0;
  IntegerLiteralExpression(std::uint64_t v, SourceRange r = {}) : Expression(NodeKind::IntegerLiteralExpression, r), value(v) {}
};

struct FloatingLiteralExpression : Expression {
  double value = 0;
  FloatingLiteralExpression(double v, SourceRange r = {}) : Expression(NodeKind::FloatingLiteralExpression, r), value(v) {}
};

struct StringLiteralExpression : Expression {
  std::string value;
  StringLiteralExpression(std::string v, SourceRange r = {}) : Expression(NodeKind::StringLiteralExpression, r), value(std::move(v)) {}
};

struct BooleanLiteralExpression : Expression {
  bool value = false;
  BooleanLiteralExpression(bool v, SourceRange r = {}) : Expression(NodeKind::BooleanLiteralExpression, r), value(v) {}
};

struct NilLiteralExpression : Expression {
  explicit NilLiteralExpression(SourceRange r = {}) : Expression(NodeKind::NilLiteralExpression, r) {}
};

struct UnaryExpression : Expression {
  tok::Kind operatorKind = tok::Kind::Unknown;
  ExpressionPtr operand;
  UnaryExpression(tok::Kind op, ExpressionPtr e, SourceRange r = {}) : Expression(NodeKind::UnaryExpression, r), operatorKind(op), operand(std::move(e)) {}
};

struct BinaryExpression : Expression {
  tok::Kind operatorKind = tok::Kind::Unknown;
  ExpressionPtr left;
  ExpressionPtr right;
  BinaryExpression(tok::Kind op, ExpressionPtr l, ExpressionPtr rr, SourceRange r = {}) : Expression(NodeKind::BinaryExpression, r), operatorKind(op), left(std::move(l)), right(std::move(rr)) {}
};

struct AssignmentExpression : Expression {
  tok::Kind operatorKind = tok::Kind::Equal;
  ExpressionPtr target;
  ExpressionPtr value;
  AssignmentExpression(tok::Kind op, ExpressionPtr l, ExpressionPtr rr, SourceRange r = {}) : Expression(NodeKind::AssignmentExpression, r), operatorKind(op), target(std::move(l)), value(std::move(rr)) {}
};

struct CallExpression : Expression {
  ExpressionPtr callee;
  std::vector<ExpressionPtr> arguments;
  explicit CallExpression(ExpressionPtr c, SourceRange r = {}) : Expression(NodeKind::CallExpression, r), callee(std::move(c)) {}
};

struct MemberExpression : Expression {
  ExpressionPtr base;
  std::string member;
  MemberExpression(ExpressionPtr b, std::string m, SourceRange r = {}) : Expression(NodeKind::MemberExpression, r), base(std::move(b)), member(std::move(m)) {}
};

struct SubscriptExpression : Expression {
  ExpressionPtr base;
  std::vector<ExpressionPtr> indices;
  explicit SubscriptExpression(ExpressionPtr b, SourceRange r = {}) : Expression(NodeKind::SubscriptExpression, r), base(std::move(b)) {}
};

struct RangeExpression : Expression {
  ExpressionPtr lower;
  ExpressionPtr upper;
  bool inclusive = true;
  RangeExpression(ExpressionPtr l, ExpressionPtr u, bool i, SourceRange r = {}) : Expression(NodeKind::RangeExpression, r), lower(std::move(l)), upper(std::move(u)), inclusive(i) {}
};

struct ArrayExpression : Expression {
  std::vector<ExpressionPtr> elements;
  explicit ArrayExpression(SourceRange r = {}) : Expression(NodeKind::ArrayExpression, r) {}
};

struct TupleExpression : Expression {
  std::vector<ExpressionPtr> elements;
  explicit TupleExpression(SourceRange r = {}) : Expression(NodeKind::TupleExpression, r) {}
};

struct SourceFile : Node {
  std::vector<std::unique_ptr<Attribute>> attributes;
  std::vector<StatementPtr> declarations;
  explicit SourceFile(SourceRange r = {}) : Node(NodeKind::SourceFile, r) {}
};

} // namespace hyperlang::ast
#endif
