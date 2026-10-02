//===--- ASTDump.cpp - HyperLang AST Debug Dump --------------------------===//
#include "hyperlang/Parse/ASTDump.h"
#include "hyperlang/Parse/TokenKinds.h"
#include <ostream>
#include <string>

namespace hyperlang::ast {
namespace {
void indent(std::ostream &out, unsigned depth) { for (unsigned i = 0; i < depth; ++i) out << "  "; }

void dumpNode(const Node &node, std::ostream &out, unsigned depth) {
  indent(out, depth);
  out << tok::getTokenName(tok::Kind::Unknown) << " ";
  switch (node.kind) {
  case NodeKind::SourceFile: out << "SourceFile"; break;
  case NodeKind::Attribute: out << "Attribute"; break;
  case NodeKind::ImportDeclaration: out << "ImportDeclaration"; break;
  case NodeKind::VariableDeclaration: out << "VariableDeclaration"; break;
  case NodeKind::FunctionDeclaration: out << "FunctionDeclaration"; break;
  case NodeKind::ParameterDeclaration: out << "ParameterDeclaration"; break;
  case NodeKind::ReturnStatement: out << "ReturnStatement"; break;
  case NodeKind::IfStatement: out << "IfStatement"; break;
  case NodeKind::WhileStatement: out << "WhileStatement"; break;
  case NodeKind::ForStatement: out << "ForStatement"; break;
  case NodeKind::GuardStatement: out << "GuardStatement"; break;
  case NodeKind::DeferStatement: out << "DeferStatement"; break;
  case NodeKind::SwitchStatement: out << "SwitchStatement"; break;
  case NodeKind::CaseClause: out << "CaseClause"; break;
  case NodeKind::BreakStatement: out << "BreakStatement"; break;
  case NodeKind::ContinueStatement: out << "ContinueStatement"; break;
  case NodeKind::ExpressionStatement: out << "ExpressionStatement"; break;
  case NodeKind::BlockStatement: out << "BlockStatement"; break;
  case NodeKind::BinaryExpression: out << "BinaryExpression"; break;
  case NodeKind::UnaryExpression: out << "UnaryExpression"; break;
  case NodeKind::AssignmentExpression: out << "AssignmentExpression"; break;
  case NodeKind::CallExpression: out << "CallExpression"; break;
  case NodeKind::MemberExpression: out << "MemberExpression"; break;
  case NodeKind::SubscriptExpression: out << "SubscriptExpression"; break;
  case NodeKind::RangeExpression: out << "RangeExpression"; break;
  case NodeKind::IdentifierExpression: out << "IdentifierExpression"; break;
  case NodeKind::IntegerLiteralExpression: out << "IntegerLiteralExpression"; break;
  case NodeKind::FloatingLiteralExpression: out << "FloatingLiteralExpression"; break;
  case NodeKind::StringLiteralExpression: out << "StringLiteralExpression"; break;
  case NodeKind::BooleanLiteralExpression: out << "BooleanLiteralExpression"; break;
  case NodeKind::NilLiteralExpression: out << "NilLiteralExpression"; break;
  case NodeKind::ArrayExpression: out << "ArrayExpression"; break;
  case NodeKind::TupleExpression: out << "TupleExpression"; break;
  case NodeKind::TypeName: out << "TypeName"; break;
  case NodeKind::Unknown: out << "Unknown"; break;
  }
  out << "\n";
}
class Dumper final : public ASTVisitor {
public:
  explicit Dumper(std::ostream &o) : out(o) {}
  void emit(Node &node) { dumpNode(node, out, depth); ASTVisitor::visit(node); }
  void visit(SourceFile &n) override { children(n, n.attributes); children(n, n.declarations); }
  void visit(BlockStatement &n) override { children(n, n.statements); }
private:
  std::ostream &out; unsigned depth = 0;
  template <typename T> void children(Node &, const std::vector<std::unique_ptr<T>> &nodes) {
    ++depth; for (const auto &n : nodes) if (n) emit(*n); --depth;
  }
};
}
void dump(const Node &node, std::ostream &output) {
  auto &mutableNode = const_cast<Node &>(node);
  Dumper dumper(output);
  dumper.emit(mutableNode);
}
}
