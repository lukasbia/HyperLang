//===--- Parser.cpp - HyperLang Recursive-Descent Parser ------------------===//

#include "hyperlang/Parse/Parser.h"
#include "hyperlang/Parse/ParseSupport.h"

#include <sstream>
#include <utility>

namespace hyperlang {

void ParserDiagnostics::error(SourceLocation l, std::string_view m) {
  diagnostics_.push_back({Diagnostic::Severity::Error, l, std::string(m)});
}
void ParserDiagnostics::warning(SourceLocation l, std::string_view m) {
  diagnostics_.push_back({Diagnostic::Severity::Warning, l, std::string(m)});
}
void ParserDiagnostics::note(SourceLocation l, std::string_view m) {
  diagnostics_.push_back({Diagnostic::Severity::Note, l, std::string(m)});
}

Parser::Parser(std::string_view source, LexerOptions options)
    : lexer_(source, options), parserDiagnostics_(diagnostics_) {}

void Parser::syncLexerDiagnostics() const {
  const DiagnosticList &lexerDiagnostics = lexer_.diagnostics();
  while (lexerDiagnosticsCount_ < lexerDiagnostics.size()) {
    diagnostics_.push_back(lexerDiagnostics[lexerDiagnosticsCount_]);
    ++lexerDiagnosticsCount_;
  }
}

const Token &Parser::current() { return lexer_.peek(); }

Token Parser::consume() { return lexer_.lex(); }

bool Parser::at(tok::Kind k) { return current().kind == k; }

bool Parser::consumeIf(tok::Kind k) {
  if (!at(k))
    return false;
  consume();
  return true;
}

bool Parser::expect(tok::Kind k, std::string_view message) {
  if (at(k)) {
    consume();
    return true;
  }
  parserDiagnostics_.error(current().range.start, message);
  return false;
}

std::string Parser::tokenText(const Token &token) const {
  if (!token.decodedText.empty())
    return token.decodedText;
  return std::string(token.text);
}

SourceRange Parser::rangeFrom(const SourceLocation &start) const {
  return {start, current().range.end};
}

void Parser::diagnoseUnexpected(std::string_view context) {
  std::string message = "unexpected ";
  message += tok::getTokenName(current().kind);
  message += " while parsing ";
  message += context;
  parserDiagnostics_.error(current().range.start, message);
}

const DiagnosticList &Parser::diagnostics() const {
  syncLexerDiagnostics();
  return diagnostics_;
}

bool Parser::hasErrors() const {
  syncLexerDiagnostics();
  if (lexer_.hasErrors())
    return true;
  for (const Diagnostic &d : diagnostics_)
    if (d.severity == Diagnostic::Severity::Error)
      return true;
  return false;
}

std::size_t Parser::offset() const { return lexer_.offset(); }

bool Parser::isDeclarationStart(tok::Kind k) const {
  return parse::isDeclarationStarter(k);
}

bool Parser::isStatementStart(tok::Kind k) const {
  return isDeclarationStart(k) || parse::isStatementStarter(k);
}

bool Parser::isTypeStart(tok::Kind k) const {
  return k == tok::Kind::Identifier ||
         k == tok::Kind::EscapedIdentifier ||
         parse::isBuiltinTypeToken(k);
}

bool Parser::isAssignmentOperator(tok::Kind k) const {
  return parse::isAssignmentOperator(k);
}

bool Parser::isUnaryOperator(tok::Kind k) const {
  return parse::isUnaryOperator(k);
}

int Parser::precedence(tok::Kind k) const {
  return parse::expressionPrecedence(k);
}

void Parser::synchronizeToDeclaration() {
  while (!at(tok::Kind::EndOfFile)) {
    if (isDeclarationStart(current().kind) ||
        isStatementStart(current().kind))
      return;
    consume();
  }
}

void Parser::synchronizeToStatement() {
  while (!at(tok::Kind::EndOfFile)) {
    if (at(tok::Kind::Semicolon) || at(tok::Kind::CloseBrace)) {
      consumeIf(tok::Kind::Semicolon);
      return;
    }
    if (isStatementStart(current().kind))
      return;
    consume();
  }
}

ParserResult Parser::parse() {
  ParserResult result;
  result.sourceFile = parseSourceFile();
  syncLexerDiagnostics();
  result.diagnostics = diagnostics_;
  return result;
}

std::unique_ptr<ast::SourceFile> Parser::parseSourceFile() {
  auto file = std::make_unique<ast::SourceFile>();
  SourceLocation start = current().range.start;

  while (!at(tok::Kind::EndOfFile)) {
    if (current().kind >= tok::Kind::AtImport &&
        current().kind <= tok::Kind::AtUnknown) {
      file->attributes.push_back(parseAttribute());
      continue;
    }

    ast::StatementPtr declaration = parseDeclaration();
    if (declaration) {
      file->declarations.push_back(std::move(declaration));
      continue;
    }

    diagnoseUnexpected("a top-level declaration");
    synchronizeToDeclaration();
  }

  file->range = rangeFrom(start);
  return file;
}

std::unique_ptr<ast::Attribute> Parser::parseAttribute() {
  Token token = consume();
  SourceLocation start = token.range.start;
  auto attribute = std::make_unique<ast::Attribute>(tokenText(token), token.range);

  if (consumeIf(tok::Kind::OpenParen)) {
    while (!at(tok::Kind::CloseParen) && !at(tok::Kind::EndOfFile)) {
      auto argument = parseExpression();
      if (!argument)
        break;
      attribute->arguments.push_back(std::move(argument));
      if (!consumeIf(tok::Kind::Comma))
        break;
    }
    expect(tok::Kind::CloseParen, "expected ')' after attribute arguments");
  }

  attribute->range = rangeFrom(start);
  return attribute;
}

ast::StatementPtr Parser::parseDeclaration() {
  if (at(tok::Kind::KwImport))
    return parseImport();
  if (at(tok::Kind::KwFunc) || at(tok::Kind::KwAsync))
    return parseFunctionDeclaration();
  if (at(tok::Kind::KwVar) || at(tok::Kind::KwLet) || at(tok::Kind::KwConst))
    return parseVariableDeclaration();
  if (at(tok::Kind::KwIf))
    return parseIfStatement();
  if (at(tok::Kind::KwWhile))
    return parseWhileStatement();
  if (at(tok::Kind::KwFor))
    return parseForStatement();
  if (at(tok::Kind::KwGuard))
    return parseGuardStatement();
  if (at(tok::Kind::KwDefer))
    return parseDeferStatement();
  if (at(tok::Kind::KwSwitch))
    return parseSwitchStatement();
  if (at(tok::Kind::KwReturn))
    return parseReturnStatement();
  if (at(tok::Kind::KwBreak)) {
    Token t = consume();
    consumeIf(tok::Kind::Semicolon);
    return std::make_unique<ast::BreakStatement>(t.range);
  }
  if (at(tok::Kind::KwContinue)) {
    Token t = consume();
    consumeIf(tok::Kind::Semicolon);
    return std::make_unique<ast::ContinueStatement>(t.range);
  }
  return parseStatement();
}

ast::StatementPtr Parser::parseStatement() {
  if (at(tok::Kind::OpenBrace))
    return parseBlock();
  if (at(tok::Kind::KwIf))
    return parseIfStatement();
  if (at(tok::Kind::KwWhile))
    return parseWhileStatement();
  if (at(tok::Kind::KwFor))
    return parseForStatement();
  if (at(tok::Kind::KwGuard))
    return parseGuardStatement();
  if (at(tok::Kind::KwDefer))
    return parseDeferStatement();
  if (at(tok::Kind::KwSwitch))
    return parseSwitchStatement();
  if (at(tok::Kind::KwReturn))
    return parseReturnStatement();

  if (!isStatementStart(current().kind))
    return nullptr;

  SourceLocation start = current().range.start;
  auto expression = parseExpression();
  if (!expression)
    return nullptr;
  consumeIf(tok::Kind::Semicolon);
  return std::make_unique<ast::ExpressionStatement>(
      std::move(expression), rangeFrom(start));
}

std::unique_ptr<ast::BlockStatement> Parser::parseBlock() {
  SourceLocation start = current().range.start;
  expect(tok::Kind::OpenBrace, "expected '{' to begin block");
  auto block = std::make_unique<ast::BlockStatement>();
  while (!at(tok::Kind::CloseBrace) && !at(tok::Kind::EndOfFile)) {
    ast::StatementPtr statement = parseDeclaration();
    if (statement) {
      block->statements.push_back(std::move(statement));
      continue;
    }
    diagnoseUnexpected("a statement");
    synchronizeToStatement();
  }
  if (!expect(tok::Kind::CloseBrace, "expected '}' to close block")) {
    block->range = rangeFrom(start);
    return block;
  }
  block->range = rangeFrom(start);
  return block;
}

std::unique_ptr<ast::ImportDeclaration> Parser::parseImport() {
  SourceLocation start = consume().range.start;
  std::string name;

  while (!at(tok::Kind::EndOfFile)) {
    if (at(tok::Kind::Identifier) || at(tok::Kind::EscapedIdentifier) ||
        at(tok::Kind::KwString) || at(tok::Kind::KwFile) ||
        at(tok::Kind::KwData) || at(tok::Kind::Dot)) {
      Token t = consume();
      name += tokenText(t);
      continue;
    }
    break;
  }

  if (name.empty())
    parserDiagnostics_.error(current().range.start, "expected module name after 'import'");

  consumeIf(tok::Kind::Semicolon);
  return std::make_unique<ast::ImportDeclaration>(name, rangeFrom(start));
}

std::unique_ptr<ast::VariableDeclaration> Parser::parseVariableDeclaration() {
  Token introducer = consume();
  SourceLocation start = introducer.range.start;
  auto declaration = std::make_unique<ast::VariableDeclaration>();
  declaration->range.start = start;
  declaration->isMutable = introducer.kind == tok::Kind::KwVar;
  declaration->isConstant = introducer.kind == tok::Kind::KwConst;

  if (!parse::isDeclarationNameToken(current().kind)) {
    parserDiagnostics_.error(current().range.start, "expected variable name");
    return declaration;
  }

  declaration->name = tokenText(consume());
  if (parse::containsUnicodeConfusable(declaration->name))
    parserDiagnostics_.warning(current().range.start, "identifier contains Unicode characters that may be visually confusable");

  if (consumeIf(tok::Kind::Colon))
    declaration->type = parseType();

  if (consumeIf(tok::Kind::Equal))
    declaration->initializer = parseExpression();

  if (!declaration->initializer && !declaration->type)
    parserDiagnostics_.error(current().range.start, "variable declaration requires a type or initializer");

  consumeIf(tok::Kind::Semicolon);
  declaration->range = rangeFrom(start);
  return declaration;
}

std::unique_ptr<ast::FunctionDeclaration> Parser::parseFunctionDeclaration() {
  SourceLocation start = current().range.start;
  auto function = std::make_unique<ast::FunctionDeclaration>("");

  if (consumeIf(tok::Kind::KwAsync))
    function->isAsync = true;

  expect(tok::Kind::KwFunc, "expected 'func'");

  if (!parse::isDeclarationNameToken(current().kind)) {
    parserDiagnostics_.error(current().range.start, "expected function name");
  } else {
    function->name = tokenText(consume());
    if (parse::containsUnicodeConfusable(function->name))
      parserDiagnostics_.warning(current().range.start, "identifier contains Unicode characters that may be visually confusable");
  }

  expect(tok::Kind::OpenParen, "expected '(' after function name");
  while (!at(tok::Kind::CloseParen) && !at(tok::Kind::EndOfFile)) {
    auto parameter = parseParameter();
    if (parameter)
      function->parameters.push_back(std::move(parameter));
    else
      break;
    if (!consumeIf(tok::Kind::Comma))
      break;
  }
  expect(tok::Kind::CloseParen, "expected ')' after parameter list");

  if (consumeIf(tok::Kind::Arrow))
    function->returnType = parseType();
  else if (consumeIf(tok::Kind::Colon))
    function->returnType = parseType();

  if (consumeIf(tok::Kind::KwThrows) || consumeIf(tok::Kind::KwThrow))
    function->isThrowing = true;

  if (at(tok::Kind::OpenBrace))
    function->body = parseBlock();
  else
    parserDiagnostics_.error(current().range.start, "expected function body");

  function->range = rangeFrom(start);
  return function;
}

std::unique_ptr<ast::ParameterDeclaration> Parser::parseParameter() {
  if (!current().isIdentifier())
    return nullptr;

  SourceLocation start = current().range.start;
  auto parameter = std::make_unique<ast::ParameterDeclaration>(tokenText(consume()));

  if (consumeIf(tok::Kind::Colon))
    parameter->type = parseType();

  if (consumeIf(tok::Kind::Equal))
    parameter->defaultValue = parseExpression();

  parameter->range = rangeFrom(start);
  return parameter;
}

std::unique_ptr<ast::IfStatement> Parser::parseIfStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::IfStatement>();
  statement->condition = parseExpression();

  consumeIf(tok::Kind::KwThen);
  if (at(tok::Kind::OpenBrace))
    statement->thenBody = parseBlock();
  else
    parserDiagnostics_.error(current().range.start, "expected '{' after if condition");

  if (consumeIf(tok::Kind::KwElse)) {
    if (at(tok::Kind::KwIf))
      statement->elseBody = std::make_unique<ast::BlockStatement>();
    else if (at(tok::Kind::OpenBrace))
      statement->elseBody = parseBlock();
    else
      parserDiagnostics_.error(current().range.start, "expected block after 'else'");
  }

  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::WhileStatement> Parser::parseWhileStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::WhileStatement>();
  statement->condition = parseExpression();
  if (at(tok::Kind::OpenBrace))
    statement->body = parseBlock();
  else
    parserDiagnostics_.error(current().range.start, "expected '{' after while condition");
  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::ForStatement> Parser::parseForStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::ForStatement>();

  if (parse::isPatternToken(current().kind))
    statement->pattern = tokenText(consume());
  else
    parserDiagnostics_.error(current().range.start, "expected loop pattern");

  expect(tok::Kind::KwIn, "expected 'in' in for statement");
  statement->sequence = parseExpression();

  if (at(tok::Kind::OpenBrace))
    statement->body = parseBlock();
  else
    parserDiagnostics_.error(current().range.start, "expected '{' after for statement");

  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::GuardStatement> Parser::parseGuardStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::GuardStatement>();
  statement->condition = parseExpression();

  if (consumeIf(tok::Kind::KwThen) || consumeIf(tok::Kind::KwElse)) {
    if (at(tok::Kind::OpenBrace))
      statement->elseBody = parseBlock();
  } else if (at(tok::Kind::OpenBrace)) {
    statement->elseBody = parseBlock();
  } else {
    parserDiagnostics_.error(current().range.start, "expected guard body");
  }

  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::DeferStatement> Parser::parseDeferStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::DeferStatement>();
  if (at(tok::Kind::OpenBrace))
    statement->body = parseBlock();
  else
    parserDiagnostics_.error(current().range.start, "expected '{' after defer");
  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::SwitchStatement> Parser::parseSwitchStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::SwitchStatement>();
  statement->subject = parseExpression();
  expect(tok::Kind::OpenBrace, "expected '{' after switch subject");

  while (!at(tok::Kind::CloseBrace) && !at(tok::Kind::EndOfFile)) {
    if (!at(tok::Kind::KwCase) && !at(tok::Kind::KwDefault)) {
      diagnoseUnexpected("switch case");
      synchronizeToStatement();
      continue;
    }
    statement->cases.push_back(parseCaseClause());
  }

  expect(tok::Kind::CloseBrace, "expected '}' after switch");
  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::CaseClause> Parser::parseCaseClause() {
  SourceLocation start = consume().range.start;
  auto clause = std::make_unique<ast::CaseClause>();

  if (current().kind != tok::Kind::KwDefault) {
    auto pattern = parseExpression();
    if (pattern)
      clause->patterns.push_back(std::move(pattern));
  }

  if (consumeIf(tok::Kind::Colon)) {
    clause->body = std::make_unique<ast::BlockStatement>();
    while (!at(tok::Kind::KwCase) && !at(tok::Kind::KwDefault) &&
           !at(tok::Kind::CloseBrace) && !at(tok::Kind::EndOfFile)) {
      auto statement = parseDeclaration();
      if (statement)
        clause->body->statements.push_back(std::move(statement));
      else {
        diagnoseUnexpected("case statement");
        synchronizeToStatement();
      }
    }
  } else if (at(tok::Kind::OpenBrace)) {
    clause->body = parseBlock();
  } else {
    parserDiagnostics_.error(current().range.start, "expected ':' or block after case");
  }

  clause->range = rangeFrom(start);
  return clause;
}

std::unique_ptr<ast::ReturnStatement> Parser::parseReturnStatement() {
  SourceLocation start = consume().range.start;
  auto statement = std::make_unique<ast::ReturnStatement>();
  if (!at(tok::Kind::Semicolon) && !at(tok::Kind::CloseBrace) &&
      !at(tok::Kind::EndOfFile))
    statement->value = parseExpression();
  consumeIf(tok::Kind::Semicolon);
  statement->range = rangeFrom(start);
  return statement;
}

std::unique_ptr<ast::TypeNode> Parser::parseType() {
  SourceLocation start = current().range.start;
  if (!isTypeStart(current().kind)) {
    parserDiagnostics_.error(current().range.start, "expected type name");
    return nullptr;
  }

  std::string name = tokenText(consume());
  auto type = std::make_unique<ast::TypeNode>(name);

  if (consumeIf(tok::Kind::Less)) {
    while (!at(tok::Kind::Greater) && !at(tok::Kind::EndOfFile)) {
      if (auto argument = parseType())
        type->arguments.push_back(std::move(argument));
      else
        break;
      if (!consumeIf(tok::Kind::Comma))
        break;
    }
    expect(tok::Kind::Greater, "expected '>' in generic type");
  }

  if (consumeIf(tok::Kind::Question))
    type->optional = true;

  type->range = rangeFrom(start);
  return type;
}

ast::ExpressionPtr Parser::parseExpression() {
  return parseAssignment();
}

ast::ExpressionPtr Parser::parseAssignment() {
  SourceLocation start = current().range.start;
  auto lhs = parseRange();
  if (!lhs)
    return nullptr;

  if (isAssignmentOperator(current().kind)) {
    tok::Kind op = consume().kind;
    auto rhs = parseAssignment();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after assignment operator");
      return lhs;
    }
    return std::make_unique<ast::AssignmentExpression>(
        op, std::move(lhs), std::move(rhs), rangeFrom(start));
  }
  return lhs;
}

ast::ExpressionPtr Parser::parseRange() {
  SourceLocation start = current().range.start;
  auto lhs = parseLogicalOr();
  if (!lhs)
    return nullptr;

  if (at(tok::Kind::DotDot) || at(tok::Kind::DotDotLess)) {
    bool inclusive = consume().kind == tok::Kind::DotDot;
    auto rhs = parseLogicalOr();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after range operator");
      return lhs;
    }
    return std::make_unique<ast::RangeExpression>(
        std::move(lhs), std::move(rhs), inclusive, rangeFrom(start));
  }
  return lhs;
}

ast::ExpressionPtr Parser::parseLogicalOr() {
  auto expression = parseLogicalAnd();
  while (at(tok::Kind::PipePipe)) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseLogicalAnd();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after operator");
      return expression;
    }
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseLogicalAnd() {
  auto expression = parseEquality();
  while (at(tok::Kind::AmpersandAmpersand)) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseEquality();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after operator");
      return expression;
    }
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseEquality() {
  auto expression = parseComparison();
  while (at(tok::Kind::EqualEqual) || at(tok::Kind::NotEqual) ||
         at(tok::Kind::TildeEqual)) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseComparison();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after operator");
      return expression;
    }
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseComparison() {
  auto expression = parseTerm();
  while (precedence(current().kind) == 40) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseTerm();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after operator");
      return expression;
    }
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseTerm() {
  auto expression = parseFactor();
  while (at(tok::Kind::ShiftLeft) || at(tok::Kind::ShiftRight) ||
         at(tok::Kind::Plus) || at(tok::Kind::Minus) ||
         at(tok::Kind::Pipe) || at(tok::Kind::Caret) ||
         at(tok::Kind::Ampersand)) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseFactor();
    if (!rhs) {
      parserDiagnostics_.error(current().range.start,
                               "expected expression after operator");
      return expression;
    }
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseFactor() {
  auto expression = parseUnary();
  while (at(tok::Kind::Star) || at(tok::Kind::Slash) ||
         at(tok::Kind::Percent)) {
    SourceLocation start = expression->range.start;
    tok::Kind op = consume().kind;
    auto rhs = parseUnary();
    expression = std::make_unique<ast::BinaryExpression>(
        op, std::move(expression), std::move(rhs), rangeFrom(start));
  }
  return expression;
}

ast::ExpressionPtr Parser::parseUnary() {
  if (!isUnaryOperator(current().kind))
    return parsePostfix();

  SourceLocation start = current().range.start;
  tok::Kind op = consume().kind;
  auto operand = parseUnary();
  if (!operand) {
    parserDiagnostics_.error(current().range.start, "expected operand after unary operator");
    return nullptr;
  }
  return std::make_unique<ast::UnaryExpression>(op, std::move(operand), rangeFrom(start));
}

ast::ExpressionPtr Parser::parsePostfix() {
  auto expression = parsePrimary();
  if (!expression)
    return nullptr;

  while (true) {
    if (at(tok::Kind::OpenParen)) {
      expression = parseCall(std::move(expression));
      continue;
    }
    if (at(tok::Kind::Dot)) {
      expression = parseMember(std::move(expression));
      continue;
    }
    if (at(tok::Kind::OpenBracket)) {
      expression = parseSubscript(std::move(expression));
      continue;
    }
    break;
  }
  return expression;
}

ast::ExpressionPtr Parser::parseCall(ast::ExpressionPtr callee) {
  SourceLocation start = callee->range.start;
  consume();
  auto call = std::make_unique<ast::CallExpression>(std::move(callee));

  while (!at(tok::Kind::CloseParen) && !at(tok::Kind::EndOfFile)) {
    auto argument = parseExpression();
    if (argument)
      call->arguments.push_back(std::move(argument));
    else
      break;
    if (!consumeIf(tok::Kind::Comma))
      break;
  }

  expect(tok::Kind::CloseParen, "expected ')' after arguments");
  call->range = rangeFrom(start);
  return call;
}

ast::ExpressionPtr Parser::parseMember(ast::ExpressionPtr base) {
  SourceLocation start = base->range.start;
  consume();
  if (!current().isIdentifier() && !isTypeStart(current().kind)) {
    parserDiagnostics_.error(current().range.start, "expected member name after '.'");
    return base;
  }
  std::string member = tokenText(consume());
  return std::make_unique<ast::MemberExpression>(
      std::move(base), std::move(member), rangeFrom(start));
}

ast::ExpressionPtr Parser::parseSubscript(ast::ExpressionPtr base) {
  SourceLocation start = base->range.start;
  consume();
  auto expression = std::make_unique<ast::SubscriptExpression>(std::move(base));
  while (!at(tok::Kind::CloseBracket) && !at(tok::Kind::EndOfFile)) {
    auto index = parseExpression();
    if (index)
      expression->indices.push_back(std::move(index));
    else
      break;
    if (!consumeIf(tok::Kind::Comma))
      break;
  }
  expect(tok::Kind::CloseBracket, "expected ']' after subscript");
  expression->range = rangeFrom(start);
  return expression;
}

ast::ExpressionPtr Parser::parseArrayLiteral() {
  SourceLocation start = consume().range.start;
  auto array = std::make_unique<ast::ArrayExpression>();
  while (!at(tok::Kind::CloseBracket) && !at(tok::Kind::EndOfFile)) {
    auto element = parseExpression();
    if (element)
      array->elements.push_back(std::move(element));
    else
      break;
    if (!consumeIf(tok::Kind::Comma))
      break;
  }
  expect(tok::Kind::CloseBracket, "expected ']' after array literal");
  array->range = rangeFrom(start);
  return array;
}

ast::ExpressionPtr Parser::parseTupleOrParenthesized() {
  SourceLocation start = consume().range.start;
  std::vector<ast::ExpressionPtr> values;

  if (consumeIf(tok::Kind::CloseParen))
    return std::make_unique<ast::TupleExpression>(rangeFrom(start));

  while (!at(tok::Kind::CloseParen) && !at(tok::Kind::EndOfFile)) {
    auto value = parseExpression();
    if (value)
      values.push_back(std::move(value));
    else
      break;
    if (!consumeIf(tok::Kind::Comma))
      break;
  }

  expect(tok::Kind::CloseParen, "expected ')'");

  if (values.size() == 1)
    return std::move(values.front());

  auto tuple = std::make_unique<ast::TupleExpression>();
  tuple->elements = std::move(values);
  tuple->range = rangeFrom(start);
  return tuple;
}

ast::ExpressionPtr Parser::parseIdentifierExpression() {
  Token token = consume();
  return std::make_unique<ast::IdentifierExpression>(
      tokenText(token), token.range);
}

ast::ExpressionPtr Parser::parseLiteral() {
  Token token = consume();
  switch (token.kind) {
  case tok::Kind::IntegerLiteral:
    return std::make_unique<ast::IntegerLiteralExpression>(
        token.integerValue, token.range);
  case tok::Kind::FloatingLiteral:
    return std::make_unique<ast::FloatingLiteralExpression>(
        token.floatingValue, token.range);
  case tok::Kind::StringLiteral:
  case tok::Kind::CharacterLiteral:
    return std::make_unique<ast::StringLiteralExpression>(
        token.decodedText.empty() ? std::string(token.text) : token.decodedText,
        token.range);
  case tok::Kind::KwTrue:
    return std::make_unique<ast::BooleanLiteralExpression>(true, token.range);
  case tok::Kind::KwFalse:
    return std::make_unique<ast::BooleanLiteralExpression>(false, token.range);
  case tok::Kind::KwNil:
  case tok::Kind::KwNull:
    return std::make_unique<ast::NilLiteralExpression>(token.range);
  default:
    return nullptr;
  }
}

ast::ExpressionPtr Parser::parsePrimary() {
  if (parse::isExpressionNameToken(current().kind))
    return parseIdentifierExpression();

  if (at(tok::Kind::IntegerLiteral) || at(tok::Kind::FloatingLiteral) ||
      at(tok::Kind::StringLiteral) || at(tok::Kind::CharacterLiteral) ||
      at(tok::Kind::KwTrue) || at(tok::Kind::KwFalse) ||
      at(tok::Kind::KwNil) || at(tok::Kind::KwNull))
    return parseLiteral();

  if (at(tok::Kind::OpenParen))
    return parseTupleOrParenthesized();

  if (at(tok::Kind::OpenBracket))
    return parseArrayLiteral();

  diagnoseUnexpected("an expression");
  return nullptr;
}

} // namespace hyperlang
