//===--- ASTVisitor.h - HyperLang AST Traversal ---------------------------===//
#ifndef HYPERLANG_PARSE_AST_VISITOR_H
#define HYPERLANG_PARSE_AST_VISITOR_H

#include "hyperlang/Parse/AST.h"

namespace hyperlang::ast {

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;

  void visit(Node &node) {
    switch (node.kind) {
    case NodeKind::SourceFile: visit(static_cast<SourceFile &>(node)); break;
    case NodeKind::Attribute: visit(static_cast<Attribute &>(node)); break;
    case NodeKind::ImportDeclaration: visit(static_cast<ImportDeclaration &>(node)); break;
    case NodeKind::VariableDeclaration: visit(static_cast<VariableDeclaration &>(node)); break;
    case NodeKind::FunctionDeclaration: visit(static_cast<FunctionDeclaration &>(node)); break;
    case NodeKind::ParameterDeclaration: visit(static_cast<ParameterDeclaration &>(node)); break;
    case NodeKind::ReturnStatement: visit(static_cast<ReturnStatement &>(node)); break;
    case NodeKind::IfStatement: visit(static_cast<IfStatement &>(node)); break;
    case NodeKind::WhileStatement: visit(static_cast<WhileStatement &>(node)); break;
    case NodeKind::ForStatement: visit(static_cast<ForStatement &>(node)); break;
    case NodeKind::GuardStatement: visit(static_cast<GuardStatement &>(node)); break;
    case NodeKind::DeferStatement: visit(static_cast<DeferStatement &>(node)); break;
    case NodeKind::SwitchStatement: visit(static_cast<SwitchStatement &>(node)); break;
    case NodeKind::CaseClause: visit(static_cast<CaseClause &>(node)); break;
    case NodeKind::BreakStatement: visit(static_cast<BreakStatement &>(node)); break;
    case NodeKind::ContinueStatement: visit(static_cast<ContinueStatement &>(node)); break;
    case NodeKind::ExpressionStatement: visit(static_cast<ExpressionStatement &>(node)); break;
    case NodeKind::BlockStatement: visit(static_cast<BlockStatement &>(node)); break;
    case NodeKind::BinaryExpression: visit(static_cast<BinaryExpression &>(node)); break;
    case NodeKind::UnaryExpression: visit(static_cast<UnaryExpression &>(node)); break;
    case NodeKind::AssignmentExpression: visit(static_cast<AssignmentExpression &>(node)); break;
    case NodeKind::CallExpression: visit(static_cast<CallExpression &>(node)); break;
    case NodeKind::MemberExpression: visit(static_cast<MemberExpression &>(node)); break;
    case NodeKind::SubscriptExpression: visit(static_cast<SubscriptExpression &>(node)); break;
    case NodeKind::RangeExpression: visit(static_cast<RangeExpression &>(node)); break;
    case NodeKind::IdentifierExpression: visit(static_cast<IdentifierExpression &>(node)); break;
    case NodeKind::IntegerLiteralExpression: visit(static_cast<IntegerLiteralExpression &>(node)); break;
    case NodeKind::FloatingLiteralExpression: visit(static_cast<FloatingLiteralExpression &>(node)); break;
    case NodeKind::StringLiteralExpression: visit(static_cast<StringLiteralExpression &>(node)); break;
    case NodeKind::BooleanLiteralExpression: visit(static_cast<BooleanLiteralExpression &>(node)); break;
    case NodeKind::NilLiteralExpression: visit(static_cast<NilLiteralExpression &>(node)); break;
    case NodeKind::ArrayExpression: visit(static_cast<ArrayExpression &>(node)); break;
    case NodeKind::TupleExpression: visit(static_cast<TupleExpression &>(node)); break;
    case NodeKind::TypeName: visit(static_cast<TypeNode &>(node)); break;
    case NodeKind::Unknown: visitUnknown(node); break;
    }
  }

  virtual void visit(SourceFile &node) { for (auto &a : node.attributes) if (a) visit(*a); for (auto &d : node.declarations) if (d) visit(*d); }
  virtual void visit(Attribute &node) { for (auto &e : node.arguments) if (e) visit(*e); }
  virtual void visit(ImportDeclaration &) {}
  virtual void visit(VariableDeclaration &node) { if (node.type) visit(*node.type); if (node.initializer) visit(*node.initializer); }
  virtual void visit(FunctionDeclaration &node) { for (auto &a : node.attributes) if (a) visit(*a); for (auto &p : node.parameters) if (p) visit(*p); if (node.returnType) visit(*node.returnType); if (node.body) visit(*node.body); }
  virtual void visit(ParameterDeclaration &node) { if (node.type) visit(*node.type); if (node.defaultValue) visit(*node.defaultValue); }
  virtual void visit(ReturnStatement &node) { if (node.value) visit(*node.value); }
  virtual void visit(IfStatement &node) { if (node.condition) visit(*node.condition); if (node.thenBody) visit(*node.thenBody); if (node.elseBody) visit(*node.elseBody); }
  virtual void visit(WhileStatement &node) { if (node.condition) visit(*node.condition); if (node.body) visit(*node.body); }
  virtual void visit(ForStatement &node) { if (node.sequence) visit(*node.sequence); if (node.body) visit(*node.body); }
  virtual void visit(GuardStatement &node) { if (node.condition) visit(*node.condition); if (node.elseBody) visit(*node.elseBody); }
  virtual void visit(DeferStatement &node) { if (node.body) visit(*node.body); }
  virtual void visit(SwitchStatement &node) { if (node.subject) visit(*node.subject); for (auto &c : node.cases) if (c) visit(*c); }
  virtual void visit(CaseClause &node) { for (auto &p : node.patterns) if (p) visit(*p); if (node.body) visit(*node.body); }
  virtual void visit(BreakStatement &) {}
  virtual void visit(ContinueStatement &) {}
  virtual void visit(ExpressionStatement &node) { if (node.expression) visit(*node.expression); }
  virtual void visit(BlockStatement &node) { for (auto &s : node.statements) if (s) visit(*s); }
  virtual void visit(BinaryExpression &node) { if (node.left) visit(*node.left); if (node.right) visit(*node.right); }
  virtual void visit(UnaryExpression &node) { if (node.operand) visit(*node.operand); }
  virtual void visit(AssignmentExpression &node) { if (node.target) visit(*node.target); if (node.value) visit(*node.value); }
  virtual void visit(CallExpression &node) { if (node.callee) visit(*node.callee); for (auto &a : node.arguments) if (a) visit(*a); }
  virtual void visit(MemberExpression &node) { if (node.base) visit(*node.base); }
  virtual void visit(SubscriptExpression &node) { if (node.base) visit(*node.base); for (auto &i : node.indices) if (i) visit(*i); }
  virtual void visit(RangeExpression &node) { if (node.lower) visit(*node.lower); if (node.upper) visit(*node.upper); }
  virtual void visit(IdentifierExpression &) {}
  virtual void visit(IntegerLiteralExpression &) {}
  virtual void visit(FloatingLiteralExpression &) {}
  virtual void visit(StringLiteralExpression &) {}
  virtual void visit(BooleanLiteralExpression &) {}
  virtual void visit(NilLiteralExpression &) {}
  virtual void visit(ArrayExpression &node) { for (auto &e : node.elements) if (e) visit(*e); }
  virtual void visit(TupleExpression &node) { for (auto &e : node.elements) if (e) visit(*e); }
  virtual void visit(TypeNode &node) { for (auto &a : node.arguments) if (a) visit(*a); }
  virtual void visitUnknown(Node &) {}
};

} // namespace hyperlang::ast
#endif
