#include "hyper/AST/TypedAST/TypedAST.h"
namespace hyper::typedast {
bool declarationHasType(const Declaration& declaration) noexcept { return declaration.type.kind != TypeKind::Unknown; }
bool declarationIsMutable(const Declaration& declaration) noexcept { return declaration.mutableValue; }
}
