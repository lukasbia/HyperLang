#include "hyper/AST/AST.h"
#include "hyper/AST/ASTContext.h"
#include "hyper/AST/ASTNode.h"
#include "hyper/AST/ASTDeclaration.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hyper::ast {

Node::~Node() = default;

std::string_view Node::kindName() const noexcept {
    return "Node";
}

const char* nodeKindName(NodeKind kind) noexcept {
    switch (kind) {
    case NodeKind::TranslationUnit: return "TranslationUnit";
    case NodeKind::Declaration: return "Declaration";
    case NodeKind::Expression: return "Expression";
    case NodeKind::Statement: return "Statement";
    case NodeKind::Type: return "Type";
    case NodeKind::Pattern: return "Pattern";
    case NodeKind::Attribute: return "Attribute";
    case NodeKind::Modifier: return "Modifier";
    case NodeKind::Unknown: return "Unknown";
    }
    return "Unknown";
}

bool isDeclaration(NodeKind kind) noexcept {
    return kind == NodeKind::Declaration;
}

bool isExpression(NodeKind kind) noexcept {
    return kind == NodeKind::Expression;
}

bool isStatement(NodeKind kind) noexcept {
    return kind == NodeKind::Statement;
}

bool isType(NodeKind kind) noexcept {
    return kind == NodeKind::Type;
}

bool isPattern(NodeKind kind) noexcept {
    return kind == NodeKind::Pattern;
}

bool contains(const SourceRange& range, std::size_t offset) noexcept {
    return offset >= range.begin.offset && offset <= range.end.offset;
}

bool precedes(
    const SourcePosition& left,
    const SourcePosition& right
) noexcept {
    return left.offset < right.offset;
}

ASTContext::ASTContext() = default;
ASTContext::~ASTContext() = default;

void ASTContext::clear() {
    nodes_.clear();
}

std::size_t ASTContext::size() const noexcept {
    return nodes_.size();
}

} // namespace hyper::ast
