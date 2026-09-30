#pragma once

#include <string_view>
#include "hyper/AST/AST.h"

namespace hyper::ast {

const char* nodeKindName(NodeKind kind) noexcept;

bool isDeclaration(NodeKind kind) noexcept;
bool isExpression(NodeKind kind) noexcept;
bool isStatement(NodeKind kind) noexcept;
bool isType(NodeKind kind) noexcept;
bool isPattern(NodeKind kind) noexcept;

} // namespace hyper::ast
