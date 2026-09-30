#include "hyper/AST/TypedAST/TypedAST.h"
namespace hyper::typedast {
bool expressionHasType(const Expression& expression) noexcept { return expression.type.kind != TypeKind::Unknown; }
bool expressionIsLValue(const Expression& expression) noexcept { return expression.lvalue; }
}
