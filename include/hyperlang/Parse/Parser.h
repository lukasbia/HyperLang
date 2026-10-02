#ifndef HYPERLANG_PARSE_PARSER_H
#define HYPERLANG_PARSE_PARSER_H
#include "hyperlang/Parse/AST.h"
#include "hyperlang/Parse/Lexer.h"
#include "hyperlang/Parse/ParserDiagnostics.h"
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
namespace hyperlang {
class Parser {
public:
  explicit Parser(std::string_view source, LexerOptions options = {});
  std::unique_ptr<ast::SourceFile> parseSourceFile();
  const DiagnosticList &diagnostics() const;
  bool hasErrors() const;
  std::size_t offset() const;
private:
  Lexer lexer_;
  DiagnosticList diagnostics_;
  ParserDiagnostics parserDiagnostics_;
  const Token &current();
  Token consume();
  bool at(tok::Kind k);
  bool consumeIf(tok::Kind k);
  bool expect(tok::Kind k, std::string_view message);
  void synchronizeToDeclaration();
  void synchronizeToStatement();
  std::unique_ptr<ast::Attribute> parseAttribute();
  std::unique_ptr<ast::ImportDeclaration> parseImport();
  ast::StatementPtr parseDeclaration();
  ast::StatementPtr parseStatement();
  std::unique_ptr<ast::BlockStatement> parseBlock();
  std::unique_ptr<ast::VariableDeclaration> parseVariableDeclaration();
  std::unique_ptr<ast::FunctionDeclaration> parseFunctionDeclaration();
  std::unique_ptr<ast::IfStatement> parseIfStatement();
  std::unique_ptr<ast::WhileStatement> parseWhileStatement();
  std::unique_ptr<ast::ForStatement> parseForStatement();
  std::unique_ptr<ast::GuardStatement> parseGuardStatement();
  std::unique_ptr<ast::DeferStatement> parseDeferStatement();
  std::unique_ptr<ast::SwitchStatement> parseSwitchStatement();
  std::unique_ptr<ast::CaseClause> parseCaseClause();
  std::unique_ptr<ast::ReturnStatement> parseReturnStatement();
  std::unique_ptr<ast::TypeNode> parseType();
  std::unique_ptr<ast::ParameterDeclaration> parseParameter();
  ast::ExpressionPtr parseExpression();
  ast::ExpressionPtr parseAssignment();
  ast::ExpressionPtr parseRange();
  ast::ExpressionPtr parseLogicalOr();
  ast::ExpressionPtr parseLogicalAnd();
  ast::ExpressionPtr parseEquality();
  ast::ExpressionPtr parseComparison();
  ast::ExpressionPtr parseTerm();
  ast::ExpressionPtr parseFactor();
  ast::ExpressionPtr parseUnary();
  ast::ExpressionPtr parsePostfix();
  ast::ExpressionPtr parsePrimary();
  ast::ExpressionPtr parseCall(ast::ExpressionPtr);
  ast::ExpressionPtr parseMember(ast::ExpressionPtr);
  ast::ExpressionPtr parseSubscript(ast::ExpressionPtr);
  ast::ExpressionPtr parseArrayLiteral();
  ast::ExpressionPtr parseTupleOrParenthesized();
  ast::ExpressionPtr parseIdentifierExpression();
  ast::ExpressionPtr parseLiteral();
  bool isStatementStart(tok::Kind) const;
  bool isDeclarationStart(tok::Kind) const;
  bool isTypeStart(tok::Kind) const;
  bool isAssignmentOperator(tok::Kind) const;
  bool isUnaryOperator(tok::Kind) const;
  int precedence(tok::Kind) const;
  std::string tokenText(const Token &) const;
  SourceRange rangeFrom(const SourceLocation &) const;
  void diagnoseUnexpected(std::string_view);
};
}
#endif
