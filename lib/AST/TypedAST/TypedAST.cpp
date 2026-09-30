#include "hyper/AST/TypedAST/TypedAST.h"
namespace hyper::typedast {
const char* nodeKindName(NodeKind kind) noexcept {
    switch (kind) { case NodeKind::Module: return "Module"; case NodeKind::Function: return "Function"; case NodeKind::Variable: return "Variable"; case NodeKind::Expression: return "Expression"; case NodeKind::Statement: return "Statement"; case NodeKind::TypeReference: return "TypeReference"; case NodeKind::Unknown: return "Unknown"; }
    return "Unknown";
}
}
