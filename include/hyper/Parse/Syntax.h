#pragma once

#include "hyper/Parse/SourceLocation.h"

#include <memory>
#include <string>
#include <vector>

namespace hyper {

enum class SyntaxKind {
    TranslationUnit, ImportDeclaration, IdentifierExpression, FunctionDeclaration,
    ReturnClause, VariableDeclaration, ExpressionStatement, TypeDeclaration,
    StructDeclaration, ClassDeclaration, EnumDeclaration, CaseStatement,
    ProtocolDeclaration, ExtensionDeclaration, CompoundStatement, IfStatement,
    WhileStatement, ForStatement, DoStatement, GuardStatement, SwitchStatement,
    ReturnStatement, ThrowStatement, DeferStatement, AssignmentExpression,
    BinaryExpression, PrefixExpression, PostfixExpression, LiteralExpression,
    CallExpression, MemberExpression, SubscriptExpression, ClosureExpression,
    TupleExpression, ArrayExpression, DictionaryExpression, OptionalType, Unknown,
    NamedType, GenericType, FunctionType, TupleType, TypeAnnotation, ParameterClause,
    Parameter, GenericParameterClause, GenericParameter, Attribute, Modifier,
    Declaration, Statement, BreakStatement, ContinueStatement, DeclarationStatement,
    Expression, StringExpression, ArrayType
};

struct SyntaxNode {
    SyntaxKind kind = SyntaxKind::Unknown;
    std::string text;
    SourceRange range;
    std::vector<std::unique_ptr<SyntaxNode>> children;

    explicit SyntaxNode(SyntaxKind kind = SyntaxKind::Unknown) : kind(kind) {}
};

const char* syntaxKindName(SyntaxKind) noexcept;

} // namespace hyper
