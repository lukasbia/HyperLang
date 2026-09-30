#include "hyper/AST/TypedAST/TypedAST.h"
namespace hyper::typedast {
bool isNumeric(TypeKind kind) noexcept { return kind == TypeKind::Int || kind == TypeKind::Float || kind == TypeKind::Double; }
bool isReference(TypeKind kind) noexcept { return kind == TypeKind::Class || kind == TypeKind::Protocol || kind == TypeKind::Array; }
}
