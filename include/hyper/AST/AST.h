#pragma once

#include "hyper/AST/ASTContext.h"
#include "hyper/AST/ASTNode.h"
#include "hyper/AST/ASTDeclaration.h"
#include "hyper/AST/ASTExpression.h"
#include "hyper/AST/ASTStatement.h"
#include "hyper/AST/ASTType.h"
#include "hyper/AST/ASTPattern.h"
#include "hyper/AST/ASTGeneric.h"
#include "hyper/AST/ASTAttribute.h"
#include "hyper/AST/ASTModifier.h"
#include "hyper/AST/ASTSourceLocation.h"
#include "hyper/AST/ASTVisitor.h"
#include "hyper/AST/ASTPrinter.h"
#include "hyper/AST/ASTEnum.h"
#include "hyper/AST/ASTStruct.h"
#include "hyper/AST/ASTClass.h"
#include "hyper/AST/ASTProtocol.h"
#include "hyper/AST/ASTExtension.h"
#include "hyper/AST/ASTImport.h"
#include "hyper/AST/ASTClosure.h"
#include "hyper/AST/ASTConcurrency.h"
#include "hyper/AST/ASTOwnership.h"
#include "hyper/AST/ASTMacro.h"

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

namespace hyper::ast {

enum class NodeKind {
    TranslationUnit,
    Declaration,
    Expression,
    Statement,
    Type,
    Pattern,
    Attribute,
    Modifier,
    Unknown
};

struct SourcePosition {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

struct SourceRange {
    SourcePosition begin;
    SourcePosition end;
};

class Node {
public:
    virtual ~Node() = default;
    virtual NodeKind kind() const noexcept = 0;
    virtual std::string_view kindName() const noexcept = 0;
};

class TranslationUnit;
class Declaration;
class FunctionDeclaration;
class VariableDeclaration;
class TypeDeclaration;
class ImportDeclaration;
class Expression;
class Statement;
class Type;
class Pattern;
class Attribute;
class Modifier;

class ASTContext {
public:
    ASTContext() = default;
    ASTContext(const ASTContext&) = delete;
    ASTContext& operator=(const ASTContext&) = delete;

    template <typename T, typename... Arguments>
    T* create(Arguments&&... arguments);

    void clear();
    std::size_t size() const noexcept;

private:
    std::vector<std::unique_ptr<Node>> nodes_;
};

const char* nodeKindName(NodeKind kind) noexcept;
bool isDeclaration(NodeKind kind) noexcept;
bool isExpression(NodeKind kind) noexcept;
bool isStatement(NodeKind kind) noexcept;
bool isType(NodeKind kind) noexcept;
bool isPattern(NodeKind kind) noexcept;
bool contains(const SourceRange& range, std::size_t offset) noexcept;
bool precedes(const SourcePosition& left, const SourcePosition& right) noexcept;

} // namespace hyper::ast
